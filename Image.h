#pragma once
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
enum allocation_type{
    NO_ALLOCATION,SELF_ALLOCATED,STB_ALLOCATED

};
typedef struct{
    int width;
    int height;int channels;
    size_t size;
    uint8_t * data;
    enum allocation_type allocation_;
}Image;
void Image_load(Image *img,const char* fname);
void Image_create(Image *img,int width,int height,int channels,bool zeroed);
void Image_save(const Image*img,const char * fname);
void Image_free(Image * img);
void Image_to_grey(const Image*orig,Image *grey);
void Image_to_sepia(const Image*orig,Image *sepia);
void Image_resize(const Image *pic,int factor);
void Image_to_ASCII(const Image*pic);
void Image_Brightness(const Image* pic, unsigned int del_b);
void Image_contrast(const Image *pic, float alpha, int beta);
void Image_Saturation(const Image *pic, float alpha, int beta);
void Image_gamma(const Image *pic, float gamma);
