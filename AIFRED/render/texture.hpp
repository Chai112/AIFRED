//
//  Render_texture.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef Render_texture_hpp
#define Render_texture_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <png.h> // LibPNG

#include "facialDetection.hpp"

using namespace AIFRED;



namespace Render
{
    namespace Texture
    {
		typedef u_int8_t colourByte;
        //GLuint loadBMP_custom(const char * imagepath);
        //GLuint image = loadBMP_custom("./my_texture.bmp");
        
        
        // must be square of 2
        const int xs = 128;
        const int ys = 128;
        
        // Open the file
        void readPNG(const char *filename);
		void initTexture();
		GLuint loadTexture(u_int8_t **greyPixels);
		
		
		
        
        void init(const char *initFilename);
		
        void loadGreyImage(const char *filename, FacialDetection::GreyImage& GreyImage);
		void scaleGreyImage(int scaleX);
		GLuint loadTexture(u_int8_t **greyPixels);
		
		struct ColourRGB
		{
			colourByte R, G, B;
			
			ColourRGB(); // auto set to 000000
			ColourRGB(colourByte inR, colourByte inG, colourByte inB);
			~ColourRGB();
			
			colourByte getGrayscale();
			void setRGB(colourByte inR, colourByte inG, colourByte inB);
		};
		
		template<typename T, int X, int Y>
		class ImageData
		{
			T data[X][Y];
		};
		
		struct Image
		{
		public:
			const int sizeX, sizeY;
			
			Image();
			Image(int inSizeX, int inSizeY);	// auto init
			~Image();
			
			//ColourRGB GetColourRGB(int x, int y);
			
			void loadPNG(const char *filename);
			void scaleImage(const int sizeX, const int sizeY);
			
			ColourRGB **data;
			//data = (colourByte*)malloc(sizeof(colourByte) * height);
			
		private:
			//void initPNG(const char *initFilename);
		};
		
		Image createImage(const char *initFilename);
		GLuint loadImage(Image &inImage);
    }
}

#endif /* Render_texture_hpp */
