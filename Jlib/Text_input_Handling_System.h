/*
This whole file should become a big thing that can be used to
help handle user input in a more versatile and friendly way than
the standard library can offer usually.
*/
#ifndef TIHS
#define TIHS
#include <stdio.h>
#include <stdlib.h>
/*
This just makes grabbing text from the user so much
easier than having to make a new buffer for printf
every single time.
note the buffer has a length of 30 chars.
The parameter is for the format specifier
(like "%s" or "%i").
*/
void* grabTextFromConsole(const char* string_format)
{
    //This is the input buffer.
    static char buffer[30] = "";
    //This scans for terminal input, fgets accepts spaces.
    fgets(buffer, 30, stdin);
    //returns the pointer to the buffer.
    return buffer;
}
#endif