import subprocess as sp
import threading
import time
import json
import os
import sys

stuff = {
  "BUILD" : "./build.py" # If you ever would need the build.... Don't think so though.
}

i = 0



CFLAGS = f"-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing  -Wno-pointer-arith -Wno-unused-parameter -nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector -fno-builtin-function -fno-builtin"
CPPFLAGS = f"-m32 -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -nostdlib -Wall -Wextra -O2 -fno-pie"
ASMFLAGS = "-f elf32"
LDFLAGS = "-m elf_i386 -g -Ikernel/include"

def cmd(cmd):
  def run():
    result = sp.run(cmd, shell=True, capture_output=True, text=True)
    print(f"Running: {cmd}")
    if result.stdout:
      print(result.stdout)
    if result.stderr:
      print(result.stderr)
    if result.returncode != 0:
      print(f"Command failed with return code {result.returncode}")
  t = threading.Thread(target=run)
  t.start()
  return t


def read(root_dir):
    for dirpath, dirnames, filenames in os.walk(root_dir):
      for file in filenames:
        if file.endswith(".json"):
          proc(os.path.join(dirpath, file))


def proc(fp):
  try:
    with open(fp, "r") as f:
      data = json.load(f)
      handle_json(data, os.path.dirname(fp))
      
  except Exception as e:
    print(f"Error reading {fp}: {e}")


def handle_json(data, path):
  global i 
  exec = False
  name = ""
  if (data.get('type') == 'exec'):
    exec = True
  if (data.get('name')):
    name = data.get('name') or os.path.splitext(os.path.basename(path))[0] # use json filename if no name specified
    
  INCLUDE = "-Ikernel/include"
  
  
  ld_items = [] # none rn
    
  for key, val in stuff.items():
    if (key in data.get('requires', [])):
      ld_items.append(val) # add the files, i also need to add it to the compilation command.
      INCLUDE += f" {val}"
    else:
      if not data.get('requires'):  # nothing specified
        break

      	
      # the required package is not built yet.. THen we need to build it.
      print(f"REQUIRED PACKAGE NOT FOUND: {name}") 
      sys.exit(-1)   	
      

  
  threads = []
  obj = []
  for root, dirs, filenames in os.walk(path):
    for file in filenames:
      filepath = os.path.join(root, file)
      try: 
        if file.endswith(".c"):
          obj_path = filepath.replace('.c', '.o')
          t = cmd(f"gcc -o {obj_path} {filepath} -c {INCLUDE} {CFLAGS}")
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".cpp"):
          obj_path = filepath.replace('.cpp', '.o')
          t = cmd(f"g++ -o {obj_path} {filepath} -c {INCLUDE} {CPPFLAGS}")
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".S"): # GAS
          obj_path = filepath.replace('.S', '.o')
          t = cmd(f"gcc -o {obj_path} {filepath} -c {INCLUDE} {CFLAGS}")
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".asm"): # NASM
          obj_path = filepath.replace('.asm', '.o')
          t = cmd(f"nasm -o {obj_path} {filepath} {INCLUDE} {ASMFLAGS}")
          threads.append(t)
          obj.append(obj_path)
      except Exception as e:
        print(f"ERROR: When building {file}: {e}")
        
  # wait for threads
  for t in threads:
    t.join()
      
  # so now we need to build all files, build C files with gcc and .cpp files with g++, .S with GAS and .asm with nasm. We need to treat rust files differently we have none rn so i'll ignore them-
  
  out = name + ".bin" if exec else name + ".o"
  
  # link if its supposed to exec and we have obj
  if exec and obj:
    sp.run(f"ld -o {out} {' '.join(obj)} {' '.join(ld_items)} {LDFLAGS}", shell=True)
  
  stuff[i] = [name, os.path.dirname(path)] # add the path to the directory.
  i += 1
  
def main():
    read(".")

if __name__ == "__main__":
  main()
