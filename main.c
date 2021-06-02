#include "Image.h"
#include "utils.h"

int main(void){
    Image one;
    Image_load(&one,"test.jpg");
    ON_ERROR_EXIT(one.data==NULL,"Error in loading image");

    Image one_grey;
    Image_to_grey(&one,&one_grey);

    Image_save(&one_grey,"test_grey.jpg");
    
    //Image_resize(&one);//Image_resize(&one);Image_resize(&one);

    Image_to_ASCII(&one_grey);   
    
    Image_free(&one);
    Image_free(&one_grey);
}