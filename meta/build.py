import subprocess as sp
import threading
import time
import json
import os
import sys
from pathlib import Path

stuff = {
  "BUILD" : "./build.py" # If you ever would need the build.... Don't think so though.
}
i = 0


build_results = {}
build_lock = threading.Lock()


CFLAGS = "-m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing -Wno-pointer-arith -Wno-unused-parameter -nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector -fno-builtin-function -fno-builtin"
CPPFLAGS = "-m32 -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -nostdlib -Wall -Wextra -O2 -fno-pie -Wno-sized-deallocation"
ASMFLAGS = "-f elf32"
LDFLAGS = "-m elf_i386 -g"

class BuildError(Exception):
    pass

def cmd(command, capture_output=True):
    def run():
        try:
            result = sp.run(command, shell=True, capture_output=capture_output, text=True)
            print(f"Running: {command}")
            
            if result.stdout:
                print(result.stdout)
            if result.stderr:
                print(result.stderr)
                
            with build_lock:
                build_results[command] = {
                    'returncode': result.returncode,
                    'stdout': result.stdout,
                    'stderr': result.stderr
                }
                
            if result.returncode != 0:
                print(f"Command failed with return code {result.returncode}")
                
        except Exception as e:
            print(f"Exception running command '{command}': {e}")
            with build_lock:
                build_results[command] = {
                    'returncode': -1,
                    'stdout': '',
                    'stderr': str(e)
                }
    
    t = threading.Thread(target=run)
    t.start()
    return t

def vjson(filepath):
    try:
        with open(filepath, "r") as f:
            content = f.read().strip()
            if not content:
                print(f"Warning: Empty JSON file {filepath}")
                return None
            
            if content.count('{') != content.count('}'):
                print(f"Error: Mismatched braces in {filepath}")
                return None
            
            if content.count('[') != content.count(']'):
                print(f"Error: Mismatched brackets in {filepath}")
                return None
                
            data = json.loads(content)
            return data
            
    except json.JSONDecodeError as e:
        print(f"JSON decode error in {filepath}: {e}")
        return None
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        return None

def read(root_dir):

    for dirpath, dirnames, filenames in os.walk(root_dir):
        for file in filenames:
            if file.endswith(".json"):
                proc(os.path.join(dirpath, file))

def proc(fp):

    data = vjson(fp)
    if data is None:
        return -1
    
    try:
        return handle_json(data, os.path.dirname(fp))
    except Exception as e:
        print(f"Error processing {fp}: {e}")
        return -1

def find_json(name, root_dir):
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for file in filenames:
            if file.endswith(".json"):
                json_path = os.path.join(dirpath, file)
                data = vjson(json_path)
                if data and data.get("name") == name:
                    return json_path
    return None

def file_exists(filepath):
    return os.path.isfile(filepath) and os.access(filepath, os.R_OK)

def create_dir(filepath):
    """Ensure output directory exists"""
    output_dir = os.path.dirname(filepath)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)

def handle_json(data, path):
    """Enhanced JSON handling with dependency resolution"""
    global stuff
    global i 
    
    if not isinstance(data, dict):
        print(f"Error: Invalid JSON structure in {path}")
        return -1
    
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
            if os.path.exists(inc_dir):
                INCLUDE += f" -I{inc_dir}"
            else:
                print(f"Warning: Include directory {inc_dir} does not exist")

    # if linker script append the linker script with -T
    ld_items = []
    linker_script = ""
    if data.get("ld"):
        linker_script = data.get("ld")
        islinker = True
    else:
        islinker = False
    
    if islinker and not file_exists(linker_script):
        print(f"Error: Linker script {linker_script} not found")
        return -1
        
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
                    result = proc(req_path)
                    if result == -1:
                        print(f"ERROR: Failed to build dependency {req_obj}")
                        return -1
                    # After building, add to ld_items if it's now in stuff
                    if req_obj in stuff:
                        ld_items.append(stuff[req_obj])
                else:
                    print(f"ERROR: While building {name} - Could not find dependency {req_obj}")
                    return -1
            except Exception as e:
                print(f"ERROR: While building {name} - Could not find or build dependency {req_obj}: {e}")
                return -1

    # compile source
    threads = []
    obj = []
    
    for root, dirs, filenames in os.walk(path):
        for file in filenames:
            filepath = os.path.join(root, file)
            
            if not file_exists(filepath):
                print(f"Warning: File {filepath} not accessible")
                continue
            
            try: 
                if file.endswith(".c"):
                    obj_path = filepath.replace('.c', '.o')
                    create_dir(obj_path)
                    compile_cmd = f"gcc -o {obj_path} {filepath} -c {CFLAGS}"
                    if INCLUDE:
                        compile_cmd += f" {INCLUDE}"
                    t = cmd(compile_cmd)
                    threads.append((t, compile_cmd, obj_path))
                    obj.append(obj_path)
                elif file.endswith(".cpp"):
                    obj_path = filepath.replace('.cpp', '.o')
                    create_dir(obj_path)
                    compile_cmd = f"g++ -o {obj_path} {filepath} -c {CPPFLAGS}"
                    if INCLUDE:
                        compile_cmd += f" {INCLUDE}"
                    t = cmd(compile_cmd)
                    threads.append((t, compile_cmd, obj_path))
                    obj.append(obj_path)
                elif file.endswith(".S"): # GAS
                    obj_path = filepath.replace('.S', '.o')
                    create_dir(obj_path)
                    compile_cmd = f"gcc -o {obj_path} {filepath} -c {CFLAGS}"
                    if INCLUDE:
                        compile_cmd += f" {INCLUDE}"
                    t = cmd(compile_cmd)
                    threads.append((t, compile_cmd, obj_path))
                    obj.append(obj_path)
                elif file.endswith(".asm"): # NASM
                    obj_path = filepath.replace('.asm', '.o')
                    create_dir(obj_path)
                    compile_cmd = f"nasm -o {obj_path} {filepath} {ASMFLAGS}"
                    if INCLUDE:
                        compile_cmd += f" {INCLUDE}"
                    t = cmd(compile_cmd)
                    threads.append((t, compile_cmd, obj_path))
                    obj.append(obj_path)
            except Exception as e:
                print(f"ERROR: When building {file}: {e}")
                return -1

    # wait for threads
    compilation_failed = False
    for t, command, obj_path in threads:
        t.join()
        
        with build_lock:
            if command in build_results:
                result = build_results[command]
                if result['returncode'] != 0:
                    print(f"Compilation failed for {obj_path}")
                    compilation_failed = True
                elif not os.path.exists(obj_path):
                    print(f"Object file {obj_path} was not created")
                    compilation_failed = True

    if compilation_failed:
        print(f"ERROR: Compilation failed for {name}")
        return -1
        
    out = name + ".bin" if exec else name + ".o"
    
    # link if its supposed to exec and we have obj
    if not obj:
        print(f"Warning: No object files generated for {name}")
        return 0

    create_dir(out)
    
    if exec and obj:
        if islinker:
           result = sp.run(f"ld -o {out} -T {linker_script} {' '.join(obj)} {' '.join(ld_items)} {LDFLAGS}", shell=True, capture_output=True, text=True)
        else:
          result = sp.run(f"ld -o {out} {' '.join(obj)} {' '.join(ld_items)} {LDFLAGS}", shell=True, capture_output=True, text=True)
        
        if result.stdout:
            print(result.stdout)
        if result.stderr:
            print(result.stderr)
            
        if result.returncode != 0:
            print(f"ERROR: Linking failed for {name}")
            return -1
            
    elif not exec and obj:
        # for non exec, just make a archive - shouldn't need to use it now anyways.
        result = sp.run(f"ar rcs {name}.a {' '.join(obj)}", shell=True, capture_output=True, text=True)
        out = name + ".a"
        
        if result.returncode != 0:
            print(f"ERROR: Archive creation failed for {name}")
            if result.stderr:
                print(result.stderr)
            return -1

    # was out file created?
    if not os.path.exists(out):
        print(f"ERROR: Output file {out} was not created")
        return -1
  
    stuff[name] = out  
    i += 1
    print(f"Successfully built {name} -> {out}")
    return 0

def clean():
    for root, dirs, files in os.walk("."):
        for file in files:
            if file.endswith((".o", ".a", ".bin")):
                try:
                    os.remove(os.path.join(root, file))
                    print(f"Cleaned: {file}")
                except Exception as e:
                    print(f"Could not clean {file}: {e}")

def main():
    if len(sys.argv) > 1 and sys.argv[1] == "clean":
        clean()
        return
    
    print("Starting build process...")
    start_time = time.time()
    
    try:
        read(".")
        elapsed = time.time() - start_time
        print(f"Build completed in {elapsed:.2f} seconds")
        
        # summary
        success_count = sum(1 for result in build_results.values() if result['returncode'] == 0)
        total_count = len(build_results)
        
        print(f"Build summary: {success_count}/{total_count} commands succeeded")
        
        if success_count < total_count:
            print("Some builds failed. Check the output above for details.")
            sys.exit(1)
            
    except KeyboardInterrupt:
        print("\nBuild interrupted by user")
        sys.exit(1)
    except Exception as e:
        print(f"Build system error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
