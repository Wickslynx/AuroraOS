import subprocess as sp
import threading
import time
import json

stuff[
  "BUILD" : "~AuroraOS/build.py" # If you ever would need the build.... Don't think so though.
]

i = 1
CFLAGS = "-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing $(INCLUDE) -Wno-pointer-arith -Wno-unused-parameter -nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector -fno-builtin-function -fno-builtin"
CPPFLAGS = "-m32 -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -nostdlib -Wall -Wextra -O2 -fno-pie $(INCLUDE)"
ASMFLAGS = "-f elf32"



def cmd(cmd):
  t = threading.Thread(target=sp.run, args=(cmd,))
  t.start()

def read(root_dir):
    for dirpath, dirnames, filenames in os.walk(root_dir):
      for file in filenames:
        if file.endswidth(".json")
        proc(os.path.join(dirpath, file)

def proc(fp):
  try:
    with open(fp, "r") as f:
      data = json.load(f)
      handle_json(data)
      
  except Exception as e:
    print(f"Error reading {fp}: {e}")

def handle_json(data, path):
  if (data['type'] == 'exec'):
    exec = True
  elif (data['name']):
    name = data['name']
  # This should be all, now we need to build it.

  ld_items[] # none rn
    
  for key, val in stuff:
    if (key in data['required']):
      ld_items.append(val)
  
  for filenames in os.walk(path):
    for file in filenames:
      try: 
        if file.endswith(".c"):
          cmd(f"gcc -o {file}.o {file} -c {CFLAGS}")
        elif file.endswith(".cpp"):
          cmd(f"g++ -o {file}.o {file} -c {CPPFLAGS}")
        elif file.endswith(".S"): # GAS
          cmd(f"gcc -o {file}.o {file} -c {CFLAGS}")
        elif file.endswith(".asm"): # NASM
          cmd(f"nasm -o {file}.o {file} -c {ASMFLAGS}")
      except Exception as e:
        print(f"ERROR: When building {file}: {e}")
        
      
      
  # so now we need to build all files, build C files with gcc and .cpp files with g++, .S with GAS and .asm with nasm. We need to treat rust files differently we have none rn so i'll ignore them-



  
  stuff[i] = [name, out]
  cmd()
  
def main():


if __name__ == "__main__":
  main()
