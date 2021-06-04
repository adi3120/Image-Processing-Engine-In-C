#include "Image.h"
#include <math.h>
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_includes/stb_image.h"
#include "stb_includes/stb_image_write.h"
#include "stb_includes/stb_image_resize.h"

int min(int a, int b)
{
    return a > b ? b : a;
}
int max(int a, int b)
{
    return a >= b ? a : b;
}
int truncate(unsigned int x)
{

    return min(255, max(0, x));
}

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

    //METHOD 1
    //formula used grayscale=(r+g+b)/3
    // for (unsigned char *p = orig->data, *pg = grey->data; p != orig->data + orig->size; p += orig->channels, pg += grey->channels)
    // {
    //     *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
    //     if (orig->channels == 4)
    //     {
    //         *(pg + 1) = *(p + 3);
    //     }
    // }

    //METHOD 2
    //formula used
    //grayscale=0.299r+0.587g+0.114b;

    for (unsigned char *p = orig->data, *pg = grey->data; p != orig->data + orig->size; p += orig->channels, pg += grey->channels)
    {
        *pg = (uint8_t)(*p * 0.299 + *(p + 1) * 0.587 + *(p + 2) * 0.114);
        if (orig->channels == 4)
        {
            *(pg + 1) = *(p + 3);
        }
    }
}
void Image_to_sepia(const Image *orig, Image *sepia)
{
    ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION && orig->channels >= 3), "The input image must have at least 3 channels.");
    Image_create(sepia, orig->width, orig->height, orig->channels, false);
    ON_ERROR_EXIT(sepia->data == NULL, "Error in creating the image");

    // Sepia filter coefficients from https://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created
    for (unsigned char *p = orig->data, *pg = sepia->data; p != orig->data + orig->size; p += orig->channels, pg += sepia->channels)
    {
        *pg = (uint8_t)fmin(0.393 * *p + 0.769 * *(p + 1) + 0.189 * *(p + 2), 255.0);       // red
        *(pg + 1) = (uint8_t)fmin(0.349 * *p + 0.686 * *(p + 1) + 0.168 * *(p + 2), 255.0); // green
        *(pg + 2) = (uint8_t)fmin(0.272 * *p + 0.534 * *(p + 1) + 0.131 * *(p + 2), 255.0); // blue
        if (orig->channels == 4)
        {
            *(pg + 3) = *(p + 3);
        }
    }
}

void Image_resize(const Image *pic, int factor)
{
    int rh = (pic->height) * (1 / factor);
    int rw = (pic->width) * (1 / factor);
    //Image_create(res, res->width, res->height, pic->channels, false);
    // ON_ERROR_EXIT(res->data == NULL, "Error in creating resized image");
    stbir_resize_uint8(pic->data, pic->width, pic->height, 0, pic->data, rw, rh, 0, pic->channels);
    stbi_write_jpg("test_resized.jpg", rw, rh, pic->channels, pic->data, 100);
}
void Image_to_ASCII(const Image *pic)
{
    unsigned char shades[11] = {'.', ',', '!', ';', '+', '*', '?', '%', '$', '#', '@'};
    unsigned char ASCII_ARRAY[pic->height][pic->width];
    int i = 0;
    int j = 0;
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {
        ASCII_ARRAY[i][j] = shades[*p / 23];
        //THE PSEUDO LOOP
        //TO CHECK BOUNDARIES
        j = j < pic->width - 1 ? j + 1 : 0;
        i = j == 0 ? i + 1 : i;
    }
    for (int h = 0; h < pic->height; h++)
    {
        for (int w = 0; w < pic->width; w++)
        {
            printf("%c", ASCII_ARRAY[h][w]);
        }
        printf("\n");
    }
}
void Image_Brightness(const Image *pic, unsigned int del_b)
{
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {
        *p = truncate(((*p) + del_b));
        *(p + 1) = truncate((*(p + 1) + del_b));
        *(p + 2) = truncate((*(p + 2) + del_b));
    }
}

void Image_contrast(const Image *pic, float alpha, int beta)
{
    int bsum = 0;
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {
        bsum += *p + *(p + 1) + *(p + 2);
    }
    float myu = bsum / pic->size;
    if (beta == 255)
        alpha = INFINITY;
    else
        alpha = (255 + beta) / (255 - beta);
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {
        *p = truncate(alpha * (*p - myu) + myu);
        *(p + 1) = truncate(alpha * (*(p + 1) - myu) + myu);
        *(p + 2) = truncate(alpha * (*(p + 2) - myu) + myu);
    }
}

void Image_Saturation(const Image *pic, float alpha, int beta)
{
    float myu = 0;
    if (beta == 255)
        alpha = INFINITY;
    else
        alpha = (255 + beta) / (255 - beta);
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {
        myu = ((*p + *(p + 1) + *(p + 2)) / 3.0);
        *p = truncate(alpha * (*p - myu) + myu);
        *(p + 1) = truncate(alpha * (*(p + 1) - myu) + myu);
        *(p + 2) = truncate(alpha * (*(p + 2) - myu) + myu);
    }
}

void Image_gamma(const Image *pic, float gamma)
{
    for (unsigned char *p = pic->data; p != pic->data + pic->size; p += pic->channels)
    {

        *p = 255 * float(pow(float(*p / 255), (gamma)));
        *(p + 1) = 255 * float(pow(float((*(p + 1)) / 255), gamma));
        *(p + 2) = 255 * float(pow((float(*(p + 2) / 255)), gamma));
    }
}
