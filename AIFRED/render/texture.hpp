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

namespace Render
{
    namespace Texture
    {
		typedef u_int8_t colourByte;
		
			// wrote myself :D no help
		template<typename T>
		void arrayHeapAllocate(T ***var, const int x, const int y);
		
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
			const int sizeX, sizeY;
			
			Image();
			Image(int inSizeX, int inSizeY);	// auto init
			~Image();
			
			void loadPNG(const char *filename);
			void cropImage(const int offsetX, const int offsetY, const int toSizeX, const int toSizeY);
			colourByte **toFDSingleScanner();
			
			ColourRGB **data;
		};
		
		void initTexture();
		
		Image createImage(const char *constructFilename);
		GLuint loadImage(Image &inImage);
    }
	
}

#endif /* Render_texture_hpp */
