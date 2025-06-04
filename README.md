
<p align="center">
<img src="docs/assets/auroraos.png" width="250" />
</p>

<p align="center">
<a href="https://wickslynx.github.io/AuroraOS">Website</a> -
<a href="https://discord.com/invite/">Discord</a> -
<a href="https://github.com/Wickslynx/AuroraOS/tree/main/docs">Documentation</a>
</p>
  
> **⚠ Warning**<br> AuroraOS is still in REALLY early development.

<br>

The Nordic Operating System...


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



> **Note** <br> Please see the [docs](https://github.com/Wickslynx/AuroraOS/tree/main/docs) folder for further explanation and examples

# Specifications:
<br>


## Memory Layout 
| Section                | Address Range       | Description |
|------------------------|--------------------|-------------|
| Bootloader            | `0x0000 - 0x3000`   | Initializes segment registers and stack. |
| Kernel Loading        | `0x1000+`           | Loads kernel into memory. |
| Video Framebuffer     | `FB_ADDR (BIOS)`    | Stores pixel data for display. |
| Heap Region           | `0x140000 - 0x540000` | Dynamic memory allocation. |
| Kernel Execution      | `0x10000+`          | Protected mode kernel execution. |

---


# Our goal:
It's not our goal to make the next windows, just learn and practice modern OS developing in modern C and C++....



# Support:
Please email me on Wickslynx@yahoo.com
I will respond as soon as possible, updates come regularly. 

> **Also see** <br> For more examples, photos and images. Visit our [docs](https://github.com/Wickslynx/AuroraOS/tree/main/docs/auroraos.md).
