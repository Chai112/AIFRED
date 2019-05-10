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

namespace V_Texture
{
    //GLuint loadBMP_custom(const char * imagepath);
    //GLuint image = loadBMP_custom("./my_texture.bmp");
    
    
    // must be square of 2
    const int xs = 128;
    const int ys = 128;
    
    // Open the file
    void read_png_file(const char *filename);
    int sum(int x, int y, int width, int height, int **greyPixels);
    
    int** read(const char *filename);
    GLuint load(int **greymap);
};

#endif /* visual_texture_hpp */
