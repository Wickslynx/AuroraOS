
User should not be able to communicate with the kernel in any way outside the terminal with sudo.

-- SCREEN --

USER -> APPLICATION -> AURO (C++) -> KERNEL (C)

--- DRIVERS --

USER -> APPLICATION -> DRIVER (C++) -> KERNEL (C)

-- TERMINAL --

USER -> TERMINAL -> KERNEL


All low-level kernel programs should be written in C, all frameworks that intended for user-use is to be written in C++-
