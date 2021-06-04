#include "Image.h"
#include "utils.h"

int main(void)
{
    Image one;
    Image_load(&one, "test.jpg");
    ON_ERROR_EXIT(one.data == NULL, "Error in loading image");

    Image one_grey;
    Image_to_grey(&one, &one_grey);

    Image_save(&one_grey, "test_grey.jpg");

    //RESIZING IMAGE NOT WORKING PLEASE SUGGEST SOMETHING!!!!
    // Image one_resized;
    // Image_resize(&one, 2);
    //Image_save(&one,"test_resized.jpg");

    //Making ASCII ART of Image
    //Image_to_ASCII(&one_grey);

    //Converting Image to sepia
    Image one_sepia;
    Image_to_sepia(&one, &one_sepia);
    Image_save(&one_sepia, "test_sepia.jpg");

    //Brightness manipulation
    Image_Brightness(&one, -64);
    Image_save(&one, "test_changed_brightness.jpg");

    //Contrast manipulation
    Image_contrast(&one, 10, 100);
    Image_save(&one, "test_contrast.jpg");

    //Saturation Manipulation
    Image_Saturation(&one,10,200);
    Image_save(&one,"test_saturation.jpg");

    //Gamma Manipulation
    Image_gamma(&one,0.1);
    Image_save(&one,"test_gamma.jpg");

    Image_free(&one);
    Image_free(&one_grey);
    Image_free(&one_sepia);
}