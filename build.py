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
CFLAGS = "-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing -Wno-pointer-arith -Wno-unused-parameter -nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector -fno-builtin-function -fno-builtin"
CPPFLAGS = "-m32 -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -nostdlib -Wall -Wextra -O2 -fno-pie"
ASMFLAGS = "-f elf32"
LDFLAGS = "-m elf_i386 -g"

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

def find_json(name, root_dir):
  for dirpath, dirnames, filenames in os.walk(root_dir):
    for file in filenames:
      if file.endswith(".json"):
        json_path = os.path.join(dirpath, file)
        try:
          with open(json_path, "r") as f:
            data = json.load(f)
            if data.get("name") == name:
              return json_path
        except Exception as e:
          print(f"Error reading {json_path}: {e}")
  return None

def handle_json(data, path):
  global i 
  exec = False
  name = ""
  if (data.get('type') == 'exec'):
    exec = True
  if (data.get('name')):
    name = data.get('name')
  else:
    name = os.path.splitext(os.path.basename(path))[0] # use directory name if no name specified

  INCLUDE = ""
  if data.get('include', True):  
    INCLUDE = "-Ikernel/include"
    # specify include dirs with include dirs, not needed.
    for inc_dir in data.get('include_dirs', []):
      INCLUDE += f" -I{inc_dir}"

  ld_items = []
  
  # Handle required dependencies
  requires = data.get("requires", [])
  for req_obj in requires:
    if req_obj in stuff:
      ld_items.append(stuff[req_obj])
    else:
      # we need to build it.
      try:
        req_path = find_json(req_obj, ".")
        if req_path:
          print(f"Building required dependency: {req_obj}")
          proc(req_path)
          # After building, add to ld_items if it's now in stuff
          if req_obj in stuff:
            ld_items.append(stuff[req_obj])
        else:
          print(f"ERROR: While building {name} - Could not find dependency {req_obj}")
          return -1
      except Exception as e:
        print(f"ERROR: While building {name} - Could not find or build dependency {req_obj}: {e}")
        return -1

  threads = []
  obj = []
  for root, dirs, filenames in os.walk(path):
    for file in filenames:
      filepath = os.path.join(root, file)
      try: 
        if file.endswith(".c"):
          obj_path = filepath.replace('.c', '.o')
          compile_cmd = f"gcc -o {obj_path} {filepath} -c {CFLAGS}"
          if INCLUDE:
            compile_cmd += f" {INCLUDE}"
          t = cmd(compile_cmd)
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".cpp"):
          obj_path = filepath.replace('.cpp', '.o')
          compile_cmd = f"g++ -o {obj_path} {filepath} -c {CPPFLAGS}"
          if INCLUDE:
            compile_cmd += f" {INCLUDE}"
          t = cmd(compile_cmd)
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".S"): # GAS
          obj_path = filepath.replace('.S', '.o')
          compile_cmd = f"gcc -o {obj_path} {filepath} -c {CFLAGS}"
          if INCLUDE:
            compile_cmd += f" {INCLUDE}"
          t = cmd(compile_cmd)
          threads.append(t)
          obj.append(obj_path)
        elif file.endswith(".asm"): # NASM
          obj_path = filepath.replace('.asm', '.o')
          compile_cmd = f"nasm -o {obj_path} {filepath} {ASMFLAGS}"
          if INCLUDE:
            compile_cmd += f" {INCLUDE}"
          t = cmd(compile_cmd)
          threads.append(t)
          obj.append(obj_path)
      except Exception as e:
        print(f"ERROR: When building {file}: {e}")
        
  # wait for threads
  for t in threads:
    t.join()
      
  out = name + ".bin" if exec else name + ".o"
  
  # link if its supposed to exec and we have obj
  if exec and obj:
    sp.run(f"ld -o {out} {' '.join(obj)} {' '.join(ld_items)} {LDFLAGS}", shell=True)
  elif not exec and obj:
    # for non exec, just make a archive - shouldn't need to use it now anyways.
    sp.run(f"ar rcs {name}.a {' '.join(obj)}", shell=True)
    out = name + ".a"
  
  stuff[name] = out  
  i += 1
  
def main():
    read(".")

if __name__ == "__main__":
  main()
