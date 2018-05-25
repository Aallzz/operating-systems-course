#ifndef MY_CIRCLE_PATTERN
#define MY_CIRCLE_PATTERN

#include <iostream>
#include <unistd.h>
#include <sys/types.h>

const char* circle[] = {"*\\\n\\/\n", 
                        "/*\n\\/\n",
                        "/\\\n*/\n", 
                        "/\\\n\\*\n"};
                        
extern "C" void  print_circle(FILE* out, int x);


#endif

