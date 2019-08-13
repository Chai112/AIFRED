//
//  Render_texture.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//  http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/
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
		
        png_byte color_type;
        png_byte bit_depth;
        png_bytep *row_pointers;
        
        GLuint textureID;
        
        png_bytep row;
        png_bytep px;
        FILE *fp;
		
		bool initialisedTexture = false;
		
			// wrote myself :D no help
		template<typename T>
		void arrayHeapAllocate(T ***var, const int x, const int y)
		{
			T **tempVar = new T*[x];
			for (int i = 0; i < x; i++)
				tempVar[i] = new T[y];
			
			*var = tempVar;
			
		}
		
		ColourRGB::ColourRGB()
		{
			R = 0;
			G = 0;
			B = 0;
		}
		
		ColourRGB::ColourRGB(colourByte inR, colourByte inG, colourByte inB)
		{
			R = inR;
			G = inG;
			B = inB;
		}
		
		ColourRGB::~ColourRGB()
		{
		}
		
		colourByte ColourRGB::getGrayscale()
		{
			return R + G + B / 3;
		}
		
		Image::Image(int inSizeX, int inSizeY) : sizeX(inSizeX),  sizeY(inSizeY) 	// manual init
		{
			if (!initialisedTexture)
				initTexture();
			
			data = new ColourRGB*[sizeX];
			for (int i = 0; i < sizeX; i++)
				data[i] = new ColourRGB[sizeY];
		}
		
		Image::~Image() 	// manual init
		{
			for (int i = 0; i < sizeX; i++)
				delete [] data[i];
			
			delete [] data;
		}
		
		void Image::loadPNG(const char *filename)
		{
			fp = fopen(filename, "rb");
			
			png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if(!png) abort();
			
			png_infop info = png_create_info_struct(png);
			if(!info) abort();
			
			if(setjmp(png_jmpbuf(png))) abort();
			
			png_init_io(png, fp);
			
			png_read_info(png, info);
			
			
			if (
				sizeX != png_get_image_width(png, info) ||
				sizeY != png_get_image_height(png, info)
				)
			{
				throw "width and height of image is not constant! Create a new Image struct.";
			}
			
			
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
			
			row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * sizeY);
			for(int y = 0; y < sizeY; y++)
				row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
			
			png_read_image(png, row_pointers);
			
				// a bit of leaking here
			png_destroy_read_struct(&png, &info, (png_infopp)0);
			
				// load rgb
			for (int x = 0; x < this->sizeX; x++)
			{
				row = row_pointers[x];
				for (int y = 0; y < this->sizeY; y++)
				{
					px = &(row[(sizeY - y) * 4]);
					float r = px[0];
					float g = px[1];
					float b = px[2];
					data[sizeY - 1 - y][x].R = r;
					data[sizeY - 1 - y][x].G = g;
					data[sizeY - 1 - y][x].B = b;
					
					
						// x and y are flipped here because of pixel arrangement of the png loader
					
						// debug
					if ((r + g + b) / 3 == 255)
					{
						data[sizeY - 1 - y][x].R = 254;
					}
				}
			}
			
				// Destruct
			
			
			for(int y = 0; y < sizeY; y++)
				delete row_pointers[y];
			
			delete row_pointers;
		}
		
		colourByte **Image::toFDSingleScanner()
		{
			colourByte **pixels = nullptr;
			
			pixels = (colourByte**)malloc(sizeof(colourByte) * sizeX * sizeY);
			for(int y = 0; y < sizeY; y++)
				pixels[y] = (colourByte*)malloc(sizeof(colourByte) * sizeX);
			for (int x = 0; x < sizeX; x++)
			{
				for (int y = 0; y < sizeY; y++)
				{
					ColourRGB pix = data[x][y];
					pixels[x][y] = data[x][y].getGrayscale();
				}
			}
			return pixels;
		}
		
		void initTexture()
		{
			
			if (initialisedTexture)
			{
				printf("\n\nTexture has already been initialised! Memory Leak will occur.\n\n");
				abort();
			}
			
			initialisedTexture = true;
			printf("Texture init!");
			
				// Create one OpenGL texture
			glGenTextures(1, &textureID);
			
				// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, textureID);
			
			
				// Give the image to OpenGL
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		
		Image createImage(const char *initFilename)
		{
			fp = fopen(initFilename, "rb");
			
			png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if(!png) abort();
			
			png_infop info = png_create_info_struct(png);
			if(!info) abort();
			
			if(setjmp(png_jmpbuf(png))) abort();
			
			png_init_io(png, fp);
			
			png_read_info(png, info);
			
			
			int sizeX      = png_get_image_width(png, info);
			int sizeY     = png_get_image_height(png, info);
			color_type = png_get_color_type(png, info);
			bit_depth  = png_get_bit_depth(png, info);
			
				// a bit of leaking here
			png_destroy_read_struct(&png, &info, (png_infopp)0);
			
			Image i(sizeX,sizeY);
			return i;
		}
		
		GLuint loadImage(Image &inImage)
		{
			int sizeX = inImage.sizeX;
			int sizeY = inImage.sizeY;
			
			
			float pixels[sizeX][sizeY][3];
			
				// output rgb
			for (int x = 0; x < sizeX; x++)
			{
				for (int y = 0; y < sizeY; y++)
				{
					int xn = sizeX-x-1;
					if (2 == 255) // debug
					{ // debug
						pixels[y][xn][0] = 1; // debug
						pixels[y][xn][1] = 0; // debug
						pixels[y][xn][2] = 0; // debug
					} // debug
					else // debug
					{ // debug
						 pixels[y][xn][0] = inImage.data[x][y].R / 255.f;
						 pixels[y][xn][1] = inImage.data[x][y].G / 255.f;
						 pixels[y][xn][2] = inImage.data[x][y].B / 255.f;
					
						
					} // debug
				}
			}
			
			 glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_FLOAT, pixels);
			
			return GL_TEXTURE_2D;
		}
		
		
    }
}
