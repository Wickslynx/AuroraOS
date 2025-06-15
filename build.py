import subprocess as sp
import threading
import time
import json
import os
stuff = {
  "BUILD" : "~AuroraOS/build.py" # If you ever would need the build.... Don't think so though.
}
i = 1
CFLAGS = "-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing $(INCLUDE) -Wno-pointer-arith -Wno-unused-parameter -nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector -fno-builtin-function -fno-builtin"
CPPFLAGS = "-m32 -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -nostdlib -Wall -Wextra -O2 -fno-pie $(INCLUDE)"
ASMFLAGS = "-f elf32"
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
  exec = False
  name = ""
  if (data.get('type') == 'exec'):
    exec = True
  if (data.get('name')):
    name = data['name']
  # This should be all, now we need to build it.
  ld_items = [] # none rn
    
  for key, val in stuff.items():
    if (key in data.get('required', [])):
      ld_items.append(val)
  
  threads = []
  obj = []
  for root, dirs, filenames in os.walk(path):
    for file in filenames:
      filepath = os.path.join(root, file)
      try: 
        if file.endswith(".c"):
          obj_path = filepath.replace('.c', '.o')
          t = cmd(f"gcc -o {obj_path} {filepath} -c {CFLAGS}")
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".cpp"):
          obj_path = filepath.replace('.cpp', '.o')
          t = cmd(f"g++ -o {obj_path} {filepath} -c {CPPFLAGS}")
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".S"): # GAS
          obj_path = filepath.replace('.S', '.o')
          t = cmd(f"gcc -o {obj_path} {filepath} -c {CFLAGS}")
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".asm"): # NASM
          obj_path = filepath.replace('.asm', '.o')
          t = cmd(f"nasm -o {obj_path} {filepath} {ASMFLAGS}")
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
    ld_cmd = f"ld -o {out} {' '.join(obj)} {' '.join(ld_items)}"
    sp.run(ld_cmd, shell=True)
  
  stuff[i] = [name, out]
  
def main():
    import sys
    read(".")

if __name__ == "__main__":
  main()
