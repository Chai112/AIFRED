//
//  visual_texture.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <png.h>

#include "texture.hpp"
using namespace V_Texture;

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

void V_Texture::read_png_file(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();
    
    png_infop info = png_create_info_struct(png);
    if(!info) abort();
    
    if(setjmp(png_jmpbuf(png))) abort();
    
    png_init_io(png, fp);
    
    png_read_info(png, info);
    
    
    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);
    
    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt
    
    if(bit_depth == 16)
        png_set_strip_16(png);
    
    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    
    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    
    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    
    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    
    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);
    
    png_read_update_info(png, info);
    
    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }
    
    png_read_image(png, row_pointers);
    
    fclose(fp);
}

int** V_Texture::read(const char *filename)
{
    
    V_Texture::read_png_file(filename);
    int *greyPixels[V_Texture::xs];
    for (int i=0; i<V_Texture::xs; i++)
        greyPixels[i] = (int *)malloc(V_Texture::ys * sizeof(int));
    
    // load rgb
    for (int x = 0; x < V_Texture::xs; x++)
    {
        png_bytep row = row_pointers[x];
        for (int y = 0; y < V_Texture::ys; y++)
        {
            png_bytep px = &(row[(V_Texture::ys - y) * 4]);
            float r = px[0];
            float g = px[1];
            float b = px[2];
            greyPixels[x][y] = (r + g + b) / 3;
        }
    }
    printf("%d\n", V_Texture::sum(10,10,50,50, greyPixels) / 16);
    
    return greyPixels;
}

GLuint V_Texture::load(int **greyPixels)
{
    float pixels[V_Texture::xs][V_Texture::ys][3];
    
    // output rgb
    for (int x = 0; x < V_Texture::xs; x++)
    {
        for (int y = 0; y < V_Texture::ys; y++)
        {
            pixels[x][y][0] = greyPixels[x][y] / 255;
            pixels[x][y][1] = greyPixels[x][y] / 255;
            pixels[x][y][2] = greyPixels[x][y] / 255;
        }
    }
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, V_Texture::xs, V_Texture::ys, 0, GL_RGB, GL_FLOAT, pixels);
    
    // Give the image to OpenGL
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    return GL_TEXTURE_2D;
}

// width 0, height 0 is a 1x1 box
int V_Texture::sum (int x, int y, int width, int height, int **greyPixels)
{
    return (greyPixels[x+width][y+height] - greyPixels[x+width][y-1] - greyPixels[x-1][y+width] + greyPixels[x-1][y-1]);
}
