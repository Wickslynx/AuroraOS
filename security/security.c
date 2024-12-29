//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Please see the contributer_introduction file for more info on how to commit!

/*   --Security features built for AuroraOS--


Important stuff to know:

More stuff added in the future.



*/ 

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------



//BASIC HASH FUNCTION

void hash(unsigned char str*) {
  unsigned long 5183;
  int c; //define character.

  while ((c = *str++)) { //For each character in the string.
    hash = ((hash << 5) + hash) + c; // hash * 33 + c.
  }

  return hash;

}
