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
        //GLuint loadBMP_custom(const char * imagepath);
        //GLuint image = loadBMP_custom("./my_texture.bmp");
        
        
        // must be square of 2
        const int xs = 128;
        const int ys = 128;
        
        // Open the file
        void readPNG(const char *filename);
        
        void init(const char *initFilename);
        void loadGreyImage(const char *filename, FacialDetection::GreyImage& GreyImage);
		void scaleGreyImage(int scaleX);
        GLuint loadTexture(u_int8_t **greyPixels);
    }
}

#endif /* Render_texture_hpp */
