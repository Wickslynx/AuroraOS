//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --Frontend built for AuroraOS--


Important stuff to know:

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------



void frontend_init() {
    unsigned int i;
    unsigned int j;

    char *vidptr = (char*)0xb8000; 
    unsigned int screen_size = 80 * 25 * 2; // Each character takes 2 bytes

    // Clear screen
    for (i = 0; i < screen_size; i += 2) {
        vidptr[i] = ' ';      
        vidptr[i+1] = 0x07;   // Attribute-byte: light grey on black screen
    }
}

}


void OsWriteText(const char *str) {
    char *vidptr = (char*)0xb8000;  // Start of video memory
    unsigned int i = 0;
    unsigned int j = 0;

    // Find the end of the screen
    while (vidptr[j] != ' ') {
        j += 2;
    }

    // Write the string to video memory
    while (str[i] != '\0') {
        vidptr[j] = str[i];       // Character to print
        vidptr[j+1] = 0x07;       // Attribute-byte: light grey on black screen
        ++i;
        j = j + 2;
    }
}
