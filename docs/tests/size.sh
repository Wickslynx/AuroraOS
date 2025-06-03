#!/bin/bash

file_to_check="../../bin/bootsect.bin"

if [ -f "$file_to_check" ]; then
  file_size=$(ls -l "$file_to_check" | awk '{print $5}')
  os_size=$(ls -l "../../auroraos.img" | awk '{print $5}')
  if [ "$file_size" -eq 512 ]; then
    echo "File '$file_to_check' is 512 bytes."
    echo "AuroraOS is ' $os_size ' bytes. "
  else
    echo "ERROR: File '$file_to_check' is not 512 bytes (size: $file_size bytes)."
  fi
else
  echo "ERROR: File '$file_to_check' not found."
fi
