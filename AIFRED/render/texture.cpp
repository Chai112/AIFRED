//
//  Render_texture.cpp
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

using namespace Render;


namespace Render
{
    namespace Texture
    {
        int width, height;
        png_byte color_type;
        png_byte bit_depth;
        png_bytep *row_pointers;
        
        GLuint textureID;
        
        png_bytep row;
        png_bytep px;
        FILE *fp;
        
        static u_int8_t *greyPixels[Render::Texture::xs];
        
        void init(const char *initFilename)
        {
            
            { // TEXTURE INIT
            
                // Create one OpenGL texture
                glGenTextures(1, &textureID);
                
                // "Bind" the newly created texture : all future texture functions will modify this texture
                glBindTexture(GL_TEXTURE_2D, textureID);
                
                
                // Give the image to OpenGL
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            } //TEXTURE INIT
            
            { // PNG INIT
                
                fp = fopen(initFilename, "rb");
                
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
                for(int y = 0; y < height; y++)
                    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
            } //PNG INIT
        }

        void readPNG(const char *filename) {
            fp = fopen(filename, "rb");
            
            png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if(!png) abort();
            
            png_infop info = png_create_info_struct(png);
            if(!info) abort();
            
            if(setjmp(png_jmpbuf(png))) abort();
            
            png_init_io(png, fp);
            
            png_read_info(png, info);
            
            png_read_image(png, row_pointers);
            
            png_destroy_read_struct(&png, NULL, NULL);
            
            fclose(fp);
        }

        void loadGreyImage(const char *filename, FacialDetection::GreyImage& GreyImage)
        {
            readPNG(filename);
            
            // load rgb
            for (int x = 0; x < GreyImage.xs; x++)
            {
                row = row_pointers[x];
                for (int y = 0; y < GreyImage.ys; y++)
                {
                    px = &(row[(ys - y) * 4]);
                    float r = px[0];
                    float g = px[1];
                    float b = px[2];
                    
                    // x and y are flipped here because of pixel arrangement of the png loader
                    GreyImage.greyMap[GreyImage.ys - 1 - y][x] = (r + g + b) / 3;
                    
                    // debug
                    if ((r + g + b) / 3 == 255)
                    {
                        GreyImage.greyMap[GreyImage.ys - 1 - y][x] = 254;
                    }
                }
            }
        }

        GLuint loadTexture(u_int8_t **greyPixels)
        {
            float pixels[Render::Texture::xs][Render::Texture::ys][3];
            
            // output rgb
            for (int x = 0; x < xs; x++)
            {
                for (int y = 0; y < ys; y++)
                {
                    if (greyPixels[x][y] == 255) // debug
                    { // debug
                        pixels[y][xs-x - 1][0] = 1; // debug
                        pixels[y][xs-x - 1][1] = 0; // debug
                        pixels[y][xs-x - 1][2] = 0; // debug
                    } // debug
                    else // debug
                    { // debug
                    pixels[y][xs-x - 1][0] = greyPixels[x][y] / 255.f;
                    pixels[y][xs-x - 1][1] = greyPixels[x][y] / 255.f;
                    pixels[y][xs-x - 1][2] = greyPixels[x][y] / 255.f;
                        
                    } // debug
                }
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, xs, ys, 0, GL_RGB, GL_FLOAT, pixels);
            
            return GL_TEXTURE_2D;
        }
    }
}
