# AuroraOS (ALPHA)
The Nordic Operating System...


# Features:
- Fully custom bootloader - Integrated filesystem - Custom GUI library - 

# Getting Started:

**1.** Build the OS:
```
$ git clone https://github.com/Wickslynx/AuroraOS
$ cd AuroraOS
$ make img
```


**2.** Run the OS with QEMU:
```
$  qemu-system-i386 --drive format=raw,file=auroraos.img -montior stdio -no-reboot -d guest_errors,int,pcall,unimp,out_asm -D debug.log
```
Remove the ``-monitor stdio`` and use ``-nographic`` to run it just in the terminal.

**Building on real hardware...**
So you are building AuroraOS on real hardware, I wouldn't recommend it but here is how.
```
$ make iso
```
Burn the auroraos.iso onto a bootable USB, this might work or not.. Good luck!


***-NOTE-***

Please see the *docs* folder for further explanation and examples

# Examples:


**Current progress:**


![Uhhh ooo, Error! Please contact me.](https://github.com/Wickslynx/AuroraOS/blob/main/docs/assets/auroraos-example-1.0.png)
![Uhhh ooo, Error! Please contact me.](https://github.com/Wickslynx/AuroraOS/blob/main/docs/assets/auroraos-example-window-1.0.png)

# Our goal:
It's not our goal to make the next windows, just learn and practice modern OS developing in modern C and C++....



# Support:
Please email me on Wickslynx@yahoo.com
I will respond as soon as possible, updates come regularly. 

***AuroraOS is still in development!***
