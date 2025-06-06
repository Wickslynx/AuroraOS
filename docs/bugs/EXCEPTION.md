# List of all exceptions I've encountered, what i did to solve it.

**``0x6``**:
``
Disassembler disagrees with translator over instruction decoding
Please report this to qemu-devel@nongnu.org
Trace 0: 0x7a47c2d6d100 [000f0000/000f3e3c/00000040/ff000000] 
check_exception old: 0xffffffff new 0x6
Trace 0: 0x7a47c2cad540 [000f0000/000fff53/00000040/ff000000] 
Trace 0: 0x7a47c2d6d100 [000f0000/000f3e3c/00000040/ff000000] 
check_exception old: 0xffffffff new 0x6 
`` 


Notes: This was the thoughest one to solve, I still don't know what I did. After a month I had to rewrite most of the bootloader to solve it.

 
 **``0xE``**:
  
``
Invalid access at addr 0xF000E000, size 4, region '(null)', reason: rejected
check_exception old: 0xffffffff new 0xe
`` 

Notes: Still not solved, probably allocating beyond bounds.
