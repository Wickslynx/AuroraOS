Weird error:

logs: 

0x000f3e3a:  insw   (%dx),%es:(%di)
0x000f3e3b:  popa
0x000f3e3c:  arpl   %bp,0x69(%bx,%si)
Disassembler disagrees with translator over instruction decoding
Please report this to qemu-devel@nongnu.org
Linking TBs 0x7a47c2d6cb00 [000f3e32] index 0 -> 0x7a47c2d6cc80 [000f3e3a]
Trace 0: 0x7a47c2d6cc80 [000f0000/000f3e3a/00000040/ff000000] 
check_exception old: 0xffffffff new 0x6
Trace 0: 0x7a47c2cad540 [000f0000/000fff53/00000040/ff000000] 
----------------
IN: 
0x000f3e3c:  arpl   %bp,0x69(%bx,%si)
Disassembler disagrees with translator over instruction decoding
Please report this to qemu-devel@nongnu.org
Trace 0: 0x7a47c2d6d100 [000f0000/000f3e3c/00000040/ff000000] 
check_exception old: 0xffffffff new 0x6
Trace 0: 0x7a47c2cad540 [000f0000/000fff53/00000040/ff000000] 
Trace 0: 0x7a47c2d6d100 [000f0000/000f3e

information:
DISK ERROR
Error code: 0x01 (Invalid command)
