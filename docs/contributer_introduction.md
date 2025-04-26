# Hello! 

Great that you are interested in helping AuroraOS grow. Under here I will provide a brief overview on how to learn the codebase and my style of writing code:

Thank you! 

Signed: @Wicks

**How do I document?**

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
* @args:
*     num1 (int): 
*     num2 (int):
*
* @raises:
*     ValueError: Division by 0.
* @returns: The sum of two integers
*/

int divide(int num1, int num2) {
    if (num1 < 0 || num2 < 0) {
        panic("Division by 0 is not allowed.") 
    }
    return num1 / num2;
}

```


