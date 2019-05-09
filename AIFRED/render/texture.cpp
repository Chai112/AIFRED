//
//  visual_texture.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "texture.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <png.h>

#define GL_TEXTURE_FILE "/Users/chaidhatchaimongkol/Downloads/uvtemplate.bmp"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;

void V_Texture::read_png_file(char *filename) {
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

GLuint V_Texture::load () {
    
    /*unsigned char header[124];
    
    FILE *fp;
    
    fp = fopen(GL_TEXTURE_FILE, "rb");
    if (fp == NULL){
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", GL_TEXTURE_FILE); getchar();
        return 0;
    }
    
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }
    
    fread(&header, 124, 1, fp);
    
    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width         = *(unsigned int*)&(header[12]);
    unsigned int linearSize     = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);
    
    
    unsigned char * buffer;
    unsigned int bufsize;
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    fclose(fp);
    
    unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return 0;
    }
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    
    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;
    
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, buffer + offset);
        
        offset += size;
        width  /= 2;
        height /= 2;
        
        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if(width < 1) width = 1;
        if(height < 1) height = 1;
        
    }
    
    free(buffer);
    
    return textureID;*/
    
    // must be square of 2
    int xs = 128;
    int ys = 128;
    
    float pixels[xs][ys][3];
    
    
    char *filename = "/Users/chaidhatchaimongkol/Downloads/t.png";
    V_Texture::read_png_file(filename);
    float greyPixels[128][128];
    float greyPixelsOut[xs][ys];
    
    // load rgb
    for (int x = 0; x < xs; x++)
    {
        png_bytep row = row_pointers[x];
        for (int y = 0; y < ys; y++)
        {
            png_bytep px = &(row[(ys - y) * 4]);
            float r = px[0];
            float g = px[1];
            float b = px[2];
            greyPixels[x][y] = (r + g + b) / 3;
            greyPixelsOut[x][y] = greyPixels[x][y];
        }
    }
    // processing
    for (int x = 0; x < xs; x++)
    {
        for (int y = 0; y < ys; y++)
        {
            // (note x = 1 so no conflict)
            if (x > 0 && y > 0)
            {
                greyPixels[x][y] = greyPixels[x - 1][y] + greyPixels[x][y - 1] - greyPixels[x - 1][y - 1] + greyPixels[x][y];
            }
        }
    }
    printf("%f\n", V_Texture::sum(10,10,50,50, greyPixels) / 16);
    // output rgb
    for (int x = 0; x < xs; x++)
    {
        for (int y = 0; y < ys; y++)
        {
            pixels[x][y][0] = greyPixelsOut[x][y] / 255;
            pixels[x][y][1] = greyPixelsOut[x][y] / 255;
            pixels[x][y][2] = greyPixelsOut[x][y] / 255;
        }
    }
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, xs, ys, 0, GL_RGB, GL_FLOAT, pixels);
    
    /*int width, height;
    unsigned char* image =
    SOIL_load_image("img.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);*/
    
    // Give the image to OpenGL
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    return GL_TEXTURE_2D;
}

// width 0, height 0 is a 1x1 box
float V_Texture::sum (int x, int y, int width, int height, float greyPixels[128][128])
{
    return (greyPixels[x+width][y+height] - greyPixels[x+width][y-1] - greyPixels[x-1][y+width] + greyPixels[x-1][y-1]);
}
