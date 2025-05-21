**Interrupt 0x10 - Video Services:**

This interrupt provides a wide range of functions for controlling the display. Here are some key categories and individual functions:

Setting Video Mode (AH = 0x00): Allows you to change the screen resolution, color depth, and graphics mode (text or graphical).


Setting Cursor Type (AH = 0x01): Controls the appearance of the text cursor (start and end scan lines).


Setting Cursor Position (AH = 0x02): Moves the text cursor to a specific row and column.


Reading Cursor Position and Type (AH = 0x03): Retrieves the current cursor position and its shape.


Reading Light Pen Position (AH = 0x04): For older systems with light pen input.


Selecting Active Display Page (AH = 0x05): In text modes, allows switching between different video memory pages.


Scrolling Active Page Up (AH = 0x06): Scrolls a region of the screen upwards.


Scrolling Active Page Down (AH = 0x07): Scrolls a region of the screen downwards.


Reading Character and Attribute at Cursor Position (AH = 0x08): Retrieves the character and its color attribute at the current cursor location in text mode.


Writing Character and Attribute at Cursor Position (AH = 0x09): Writes a character with a specified color attribute at the current cursor position in text mode.


Writing Character at Cursor Position (AH = 0x0A): Writes a character at the current cursor position, using the current attribute.


Setting Palette Registers (AH = 0x0B): Controls the color palette in certain graphics modes.


Writing a String (AH = 0x0E): The "Teletype Output" function, which displays a character and advances the cursor. This is often used for basic output in bootloaders.


Getting Video Mode Information (AH = 0x0F): Retrieves information about the current video mode.


Getting Current Video State (AH = 0x1A): Returns information about the active video adapter.


Interrupt 0x13 - Disk Services:




This interrupt provides functions for interacting with disk drives (floppy and hard disks). Here are some fundamental services:


Reset Disk System (AH = 0x00): Resets the disk controller.


Read Sectors from Disk (AH = 0x02): Reads one or more sectors from the specified disk, cylinder, head, and sector into a buffer in memory. This is crucial for loading the operating system kernel.


Write Sectors to Disk (AH = 0x03): Writes one or more sectors from a memory buffer to the specified disk location.


Verify Sectors (AH = 0x04): Checks if the specified disk sectors can be read correctly.


Format Track (AH = 0x05): Formats a specific track on a disk (less commonly used by OS bootloaders).


Format Bad Track (AH = 0x06): Formats a known bad track (less common).


Get Drive Parameters (AH = 0x08): Retrieves information about the selected disk drive, such as the number of cylinders, heads, and sectors per track. This is essential for correctly addressing sectors.


Initialize Drive Parameters (AH = 0x0A): Sets drive parameters (less common).


Read Long Sectors (AH = 0x0C): Reads sectors with extended error correction (older systems).


Write Long Sectors (AH = 0x0D): Writes long sectors (older systems).


Seek (AH = 0x0E): Moves the disk's read/write head to a specific cylinder.


Get Drive Type (AH = 0x15): Determines the type of drive (floppy, hard disk, etc.).


Get Extended Drive Parameters (AH = 0x41): Retrieves more detailed information about hard disk drives (often used for larger drives).


Extended Read Sectors (AH = 0x42): An extension to the basic read function, often used for LBA (Logical Block Addressing) which simplifies addressing on larger drives.


Extended Write Sectors (AH = 0x43): Extended write function for LBA.
