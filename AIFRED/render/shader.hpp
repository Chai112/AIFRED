//
//  shader.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

class V_Shader
{
public:
    /*    "in vec3 vp;"
     "layout(location = 0) in vec3 vertexPosition_modelspace;"
     ""
     "uniform mat4 MVP;"
     ""
     "void main(){"
     "   gl_Position =  MVP * vec4(vertexPosition_modelspace,1.0);"
     #version 330 core*/
    const char* vertex_shader =
    "#version 400\n"
    
    // Input vertex data, different for all executions of this shader.
    "layout(location = 0) in vec3 vertexPosition_modelspace;"
    "layout(location = 1) in vec2 vertexUV;"
    
    // Output data ; will be interpolated for each fragment.
    "out vec2 UV;"
    
    // Values that stay constant for the whole mesh.
    "uniform mat4 MVP;"
    
    "void main(){"
        
        // Output position of the vertex, in clip space : MVP * position
        "gl_Position =  MVP * vec4(vertexPosition_modelspace,1);"
        
        // UV of the vertex. No special space for this one.
        "UV = vertexUV;"
    "}";
    
    const char* fragment_shader =
    "#version 400\n"
    "in vec2 UV;"
    "out vec3 color;"
    "uniform sampler2D myTextureSampler;"
    "void main() {"
    "   color = texture( myTextureSampler, UV ).rgb;"
    "}";
    
    GLuint vbo = 0;
    GLuint vao = 0;
    GLuint vuv = 0;
    GLuint vs, fs, shader_programme;
    
    GLFWwindow* window;
    
    int init ();
    void update ();
};

#endif /* shader_hpp */
