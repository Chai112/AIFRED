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
		
		struct ColourRGB
		{
			colourByte R, G, B;
			
			ColourRGB(); // auto set to 000000
			ColourRGB(colourByte inR, colourByte inG, colourByte inB);
			~ColourRGB();
			
			colourByte getGrayscale();
			void setRGB(colourByte inR, colourByte inG, colourByte inB);
		};
		
		struct Image
		{
		public:
			const int sizeX, sizeY;
			
			Image();
			Image(int inSizeX, int inSizeY);	// auto init
			~Image();
			
			void loadPNG(const char *filename);
			void scaleImage(const int sizeX, const int sizeY);
			
			ColourRGB **data;
		};
		
		void initTexture();
		
		Image createImage(const char *initFilename);
		GLuint loadImage(Image &inImage);
    }
}

#endif /* Render_texture_hpp */
