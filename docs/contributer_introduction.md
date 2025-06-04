<p align="center">
<img src="assets/auroraos.png" width="250" />
</p>

> **⚠ Warning**<br> This page is still under development!

## Hello!

Great that you are interested in helping AuroraOS grow. Under here I will provide a brief overview on how to learn the codebase and my style of writing code:

Thank you! 

Signed: @Wicks

## **How do I document?**

Rules:
- Always add documentation for every commit you make, if you do not document your pull-request will not be merged.

Example of a proper file documentation:


```c
/*
 * Copyright (C) 2025 AuroraOS
 * Written by Wicks, 2024
 * Modified by Wicks, 2025
 */

#include "error.h"

/* 
Returns the division of two nums.
raises: (PANIC) Division by 0 is not allowed.. (When you divide something with 0...)
*/

int divide(int num1, int num2) {
    if (num1 < 0 || num2 < 0) {
        panic("Division by 0 is not allowed."); // TODO: Change this to a warning, gracefully handle it.
    }

    return num1 / num2; // NOTE: What if you don't provide two nums?
}

```
> **✉ Note** <br> Feel free to add documentation to functions which dosen't have it, even though they are not yours.

