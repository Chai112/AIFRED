//
//  visual_texture.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef visual_texture_hpp
#define visual_texture_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW helper library

class V_Texture
{
public:
    //GLuint loadBMP_custom(const char * imagepath);
    //GLuint image = loadBMP_custom("./my_texture.bmp");
    
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
    
    // Open the file
    GLuint load ();
};

#endif /* visual_texture_hpp */
