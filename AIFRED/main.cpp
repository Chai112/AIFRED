//
//  main.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 22/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include <stdarg.h>
#include <assert.h>

#include "input/mouseInput.hpp"

#include "AIFRED/facialDetection.hpp"

#include "render/texture.hpp"
#include "render/shader.hpp"

#include "debug.hpp"

using namespace AIFRED;
using namespace Render;
//using namespace Input::Mouse;

Render::Shader shader;

Debug debug;

void glfw_error_callback(int error, const char* description) {
    debug.gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Input::Mouse::xPos = xpos;
    //Input::Mouse::yPos = ypos;
}


int main() {
    assert(debug.restart_gl_log());
    // start GL context and O/S window using the GLFW helper library
    debug.gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    // register the error call-back function that we wrote, above
    glfwSetErrorCallback(glfw_error_callback);
    
    // uncomment these lines if on Apple OS xMouse
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    if (shader.init() == 1)
        return 1;
    
    //if (Input::Mouse::init(shader.window) == 1)
        //return 1;
    

    bool init = false;
    
    // Get a handle for our "myTextureSampler" uniform
    GLuint TextureID  = glGetUniformLocation(shader.shader_programme, "myTextureSampler");
    
    char *filename = "/Users/chaidhatchaimongkol/Downloads/t.png";
	
    Texture::init(filename);
	FacialDetection::init();
	
    u_int8_t** greyPixels = Texture::loadPixels(filename);
    greyPixels = FacialDetection::process(greyPixels);
    GLuint Texture = Texture::loadTexture(greyPixels);
    
    while(!glfwWindowShouldClose(shader.window)) {
        if (GLFW_PRESS == glfwGetKey(shader.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(shader.window, 1);
        }
        
        glfwSetCursorPosCallback(shader.window, cursor_position_callback);
        
        /*unsigned char pick_col[3][100][100];
        glReadPixels(841 , 270 , 100 , 100 , GL_RGB , GL_UNSIGNED_BYTE , pick_col);
        if (pick_col[0][0][0] + pick_col[1][0][0] + pick_col[2][0][0] == 0)
        {
            printf("a\n");
        }*/
        
        greyPixels = Texture::loadPixels(filename);
        greyPixels = FacialDetection::process(greyPixels);
        Texture = Texture::loadTexture(greyPixels);
        
        
        
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, shader.vbo);
        glVertexAttribPointer(
                              0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                              3,                  // size
                              GL_FLOAT,           // type
                              GL_FALSE,           // normalized?
                              0,                  // stride
                              (void*)0            // array buffer offset
                              );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, shader.vuv);
        glVertexAttribPointer(
                              1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                              2,                                // size : U+V => 2
                              GL_FLOAT,                         // type
                              GL_FALSE,                         // normalized?
                              0,                                // stride
                              (void*)0                          // array buffer offset
                              );
        shader.update();
        
        //glDeleteTextures(1, &Render::Texture::textureID);
        
        
        if (!init) { init = true; debug.gl_log("\nInitialised Successfully. %s %s \n", __DATE__, __TIME__);}
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

