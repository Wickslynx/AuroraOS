
Invalid read at addr 0x4D0FFAB6, size 1, region '(null)', reason: rejected
Invalid write at addr 0x4D0FFAB6, size 1, region '(null)', reason: rejected
check_exception old: 0xffffffff new 0xd
     0: v=0d e=0424 i=0 cpl=0 IP=0008:00010e4f pc=00010e4f SP=0010:00017958 env->regs[R_EAX]=00000329
EAX=00000329 EBX=00010e4a ECX=ffffff35 EDX=00000000
ESI=000000b6 EDI=00000017 EBP=00000005 ESP=00017958
EIP=00010e4f EFL=00000002 [-------] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]
SS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
FS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
LDT=0000 00000000 0000ffff 00008200 DPL=0 LDT
TR =0000 00000000 0000ffff 00008b00 DPL=0 TSS32-busy
GDT=     00007dd0 00000017
IDT=     0001a280 000007ff
CR0=00000013 CR2=00000000 CR3=00000000 CR4=00000600
DR0=00000000 DR1=00000000 DR2=00000000 DR3=00000000 
DR6=ffff0ff0 DR7=00000400
CCS=00000016 CCD=0000000b CCO=SARB
EFER=0000000000000000
check_exception old: 0xd new 0xd
     1: v=08 e=0000 i=0 cpl=0 IP=0008:00010e4f pc=00010e4f SP=0010:00017958 env->regs[R_EAX]=00000329
EAX=00000329 EBX=00010e4a ECX=ffffff35 EDX=00000000
ESI=000000b6 EDI=00000017 EBP=00000005 ESP=00017958
EIP=00010e4f EFL=00000002 [-------] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]
SS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
FS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
LDT=0000 00000000 0000ffff 00008200 DPL=0 LDT
TR =0000 00000000 0000ffff 00008b00 DPL=0 TSS32-busy
GDT=     00007dd0 00000017
IDT=     0001a280 000007ff
CR0=00000013 CR2=00000000 CR3=00000000 CR4=00000600
DR0=00000000 DR1=00000000 DR2=00000000 DR3=00000000 
DR6=ffff0ff0 DR7=00000400
CCS=00000016 CCD=0000000b CCO=SARB
EFER=0000000000000000
check_exception old: 0x8 new 
