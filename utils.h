#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

//first we will write an error utility macro
// that will receive a condition and an error message

//error utility macro
#define ON_ERROR_EXIT(cond,message)\
do{\
    if((cond)){\
        printf("ERROR in funtion: %s at line %d\n",__func__,__LINE__);\
        perror((message));\
        exit(1);\
    }\
} while(0)


//CHECK IF A STRING "str" ENDS WITH A SUBSTRING "ends"
//BASICALLY TO CHECK IF A STRING IS .jpg/.JPEG.JPG/.PNG/.png

static inline bool str_ends_in(const char*str,const char*ends){
    size_t str_len=strlen(str);
    size_t ends_len=strlen(ends);
    char * pos=strstr(str,ends);
    return(pos!=NULL)&&(pos+ends_len==str+str_len);
}
