#include "Image.h"
#include <math.h>
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_includes/stb_image.h"
#include "stb_includes/stb_image_write.h"
#include "stb_includes/stb_image_resize.h"

void Image_load(Image *img, const char *fname)
{
    if ((img->data = stbi_load(fname, &img->width, &img->height, &img->channels, 0)) != NULL)
    {
        img->size = (img->width) * (img->height) * (img->channels);
        img->allocation_ = STB_ALLOCATED;
    }
}
void Image_create(Image *img, int width, int height, int channels, bool zeroed)
{
    size_t size = width * height * channels;
    if (zeroed)
    {
        img->data = (unsigned char *)calloc(size, 1);
    }
    else
    {
        img->data = (unsigned char *)malloc(size);
    }
    if (img->data != NULL)
    {
        img->width = width;
        img->height = height;
        img->size = size;
        img->channels = channels;
        img->allocation_ = SELF_ALLOCATED;
    }
}
// IN order to implement save function
//  we need some utility function
//  that we will write in a separate header file named utils.h

void Image_save(const Image *img, const char *fname)
{
    //check if the file name ends in .jpg/.JPEG/.JPG/.jpeg
    if (str_ends_in(fname, ".jpg") || str_ends_in(fname, ".jpeg") || str_ends_in(fname, ".JPEG") || str_ends_in(fname, ".JPG"))
    {
        stbi_write_jpg(fname, img->width, img->height, img->channels, img->data, 100);
    }
    else if (str_ends_in(fname, ".png") || str_ends_in(fname, ".PNG"))
    {
        stbi_write_png(fname, img->width, img->height, img->channels, img->data, img->width * img->channels);
    }
    else
    {
        ON_ERROR_EXIT(false, "");
    }
}
void Image_free(Image *img)
{
    //the image free() will check
    // if the images are allocated
    // if yes it will release the image data and zero the remaining fields
    if (img->allocation_ != NO_ALLOCATION && img->data != NULL)
    {
        if (img->allocation_ == STB_ALLOCATED)
        {
            stbi_image_free(img->data);
        }
        else
        {
            free(img->data);
        }
        img->data = NULL;
        img->height = 0;
        img->width = 0;
        img->size = 0;
        img->allocation_ = NO_ALLOCATION;
    }
}

//Image to grey implementation
//we will create a new image
//and loop all the pixel values
//to calculate the grey pixel values

void Image_to_grey(const Image *orig, Image *grey)
{
    int channels = orig->channels == 4 ? 2 : 1;
    Image_create(grey, orig->width, orig->height, channels, false);
    ON_ERROR_EXIT(grey->data == NULL, "Error in creating grey image");

    //THE LOOP

    for (unsigned char *p = orig->data, *pg = grey->data; p != orig->data + orig->size; p += orig->channels, pg += grey->channels)
    {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
        if (orig->channels == 4)
        {
            *(pg + 1) = *(p + 3);
        }
    }
}
void Image_resize(const Image *pic)
{
    int rh;
    int rw;

    rh = pic->height / 3;
    rw = pic->width / 2;

    stbir_resize_uint8(pic->data, pic->width, pic->height, 0, pic->data, rw, rh, 0, pic->channels);

    stbi_write_jpg("test.jpg", rw, rh, pic->channels, pic->data, 100);
}
void Image_to_ASCII(const Image *pic)
{
    unsigned char b[11] = {'.', ',', '!', ';', '+', '*', '?', '%', '$', '#', '@'};
    unsigned char a[pic->width][pic->height];
    int i=0;int j=0;
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {
        a[i][j] = (uint8_t)(b[*p / 23]);
        j=j<pic->width-1?j+1:0;
        i=j==0?i+1:i;
    }
    for (int h = 0; h < pic->height; h++)
    {
        for (int w = 0; w < pic->width; w++)
        {
            printf("%c", a[h][w]);
        }
    printf("\n");
    }
}
