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

    //Image_to_ASCII(&one_grey);
    Image one_sepia;
    Image_to_sepia(&one,&one_sepia);
    Image_save(&one_sepia,"test_sepia.jpg");   
    Image one_new;
    Image_Brightness(&one,-20);
    Image_save(&one,"test_bright.jpg");
    Image_free(&one);
    Image_free(&one_grey);
}