#ifndef ERROR_H
#define ERROR_H

#include "system.h"
#include "../ui/system/screen.h"
#include "../ui/system/font.h"


/*
@arguments: err, error message to write to the screen.
@returns: N/A
*/
void panic(const char *err);


#endif //ERROR_H
