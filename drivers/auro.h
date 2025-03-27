//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*  ---- AuroraOS's windowing system ----


Important stuff to know:

Color values are declared in:
utils/macros/macros.h

Ground Up I/O is defined in io/io.h

Update cursor func declared in:
utils/funcs/functions.h

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

#include "io.h"
#include "kernel/fb/fb.h"

typdef struct {
  uint16 id; //WINDOW ID
  uint16 x, y; //X, Y
  uint16 width, heigh; //HEIGHT, WIDTH
  Pixel *buffer; //Framebuffer
  uint8 z; //Layering (How it is gonna be drawn)
} Window;
  
typedef struct {
  uint16 id,
  Window windows[MAX_WIN]
  uint8 w_count
  char name[64];
} AuroApplication;


void AuroWindowCreate() {
  
}


void AuroDisplayOpen() {
    
}

void AuroEventProcess() {
  
} 

void AuroSwapBuffers() {
  
}







