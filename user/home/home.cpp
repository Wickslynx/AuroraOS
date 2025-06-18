// For now i'll provide a brief overlay over how the UI should look.

#include <auro.h>
#include <keyboard.h>
#include <util.h>
#include "bg.h" // this is the background


int __home() { // will be main later 
  //Auro::Internal::drawRectangle(COLOR(0, 0, 60), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0); // bg
  //Auro::Internal::drawText("12:00", SCREEN_WIDTH-SCREEN_WIDTH/8, SCREEN_HEIGHT/41, COLOR(0, 0, 0), 3); // time
  //Auro::Internal::drawRectangle(COLOR(236, 236, 236), SCREEN_WIDTH/20*17.9, SCREEN_HEIGHT/16,  SCREEN_WIDTH/10, SCREEN_HEIGHT/10*9, 15); // right top down bar. (files, Terminal, Notes)
  //Auro::Internal::drawRectangle(COLOR(255, 255, 255), SCREEN_WIDTH/20*0.2, SCREEN_HEIGHT/16, SCREEN_WIDTH/10, SCREEN_HEIGHT/10*9, 15); // left top-down bar. (apps)
  image_bmp(bg_bmp, bg_bmp_len, 0, 0);
  //Auro::Internal::drawRectangle(COLOR(67, 67, 67), SCREEN_WIDTH, SCREEN_HEIGHT )// this should be able to be turned off, this is the ASK AI bar.
  return 0;
} 
