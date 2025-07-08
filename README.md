
<p align="center">
<img src="docs/assets/auroraos.png" width="250" />
</p>

<p align="center">
<a href="https://discord.com/invite/3X6WzRFUVP">Discord</a> -
<a href="https://github.com/Wickslynx/AuroraOS/tree/main/docs">Documentation</a>
</p>
  
> **⚠ Warning**<br> AuroraOS is still in REALLY early development, sometimes it doesn't even boot. Don't let it come near any production environment. Stay safe!

<br>

The Nordic Operating System...


## Getting Started:

**1.** Build the OS:
```
$ git clone https://github.com/Wickslynx/AuroraOS
$ cd AuroraOS
$ make img
```


**2.** Run the OS with QEMU:
```
$  qemu-system-i386 --drive format=raw,file=auroraos.img -serial stdio -no-reboot -d guest_errors,int,pcall,unimp,out_asm -D debug.log
```
Remove the ``-monitor stdio`` and use ``-nographic`` to run it just in the terminal.

> **Advice:** <br> Use `make run` to build and run the OS.

**Building on real hardware...**
So you are building AuroraOS on real hardware, I wouldn't recommend it but here is how.
```
$ make iso
```
Burn the auroraos.iso onto a bootable USB, this might work or not.. Good luck!



> **✉ Note** <br> Please see the [docs](https://github.com/Wickslynx/AuroraOS/tree/main/docs) folder for further explanation and examples



## Our goal:
It's not our goal to make the next windows, just learn and practice modern OS developing in modern C and C++....



## Support:
Please email me on Wickslynx@yahoo.com
I will respond as soon as possible, updates come regularly. 

> **★ Also see** <br> Our [docs](https://github.com/Wickslynx/AuroraOS/tree/main/docs/auroraos.md) for more examples, images and information.
