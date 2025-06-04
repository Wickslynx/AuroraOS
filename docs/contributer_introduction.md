<p align="center">
<img src="assets/auroraos.png" width="250" />
</p>

> **⚠ Warning**<br> This page is still under development!

## **Skill dosen't matter, or does it?**
Do you really need to be a rocket scientist to contribute to AuroraOS? I don't belive so. 

Here's some things you can do at any level:
- Translating AuroraOS into different languages.
- Taking screenshots for our documents.
<br>

Here's some things you can do if you know a bit about code:
- Documenting undocumented code in our drivers.
- Making small apps. (I appreciate all apps, even "Hello World" ones.)

<br>
Here's some things you can do if you know a a lot about code:
- Designing frontend libraries, writing additional apps.
- Closing issues. 

<br>
Here's some things you can do if you know too much about code:
- Porting to different architectures.
- Working in the kernel, implementing new features as you like. 

## **Reading my codebase?**


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

