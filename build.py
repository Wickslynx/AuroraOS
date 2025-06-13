import subprocess as sp
import threading
import time
import json

stuff[
  "BUILD" : "~AuroraOS/build.py" # If you ever would need the build.... Don't think so though.
]

i = 1


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

def handle_json(data):
  if (data['type'] == 'exec'):
    exec = True
  elif (data['name']):
    name = data['name']
  # This should be all, now we need to build it.

  ld_items[] # none rn
    
  for key, val in stuff:
    if (key in data['required']):
      ld_items.append(val)
      
      
  # so now we need to build all files, build C files with gcc and .cpp files with g++, .S with GAS and .asm with nasm. We need to treat rust files differently we have none rn so i'll ignore them-



  
  stuff[i] = [name, out]
  cmd()
  
def main():


if __name__ == "__main__":
  main()
