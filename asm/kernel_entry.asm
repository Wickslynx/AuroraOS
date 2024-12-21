[bits 32]
[external kernel_main]

section .txt
global _start

_start:
  cli    ; Clear inputs.
  mov esp, 0x90000    ; Make stack.
  call kernel_main  ;  Call the kernel_main function.
  hlt ; Halt cpu.
