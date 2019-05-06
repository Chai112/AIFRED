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

#include "render/texture.hpp"
#include "render/shader.hpp"

#include "debug.hpp"

MouseIn mouseIn;

V_Texture texture;
V_Shader shader;

Debug debug;

#define GL_LOG_FILE "/Users/chaidhatchaimongkol/Documents/ESC/ESC/log/gl.log"
#define GL_TEXTURE_FILE "/Users/chaidhatchaimongkol/Documents/ESC/gl.bmp"

void glfw_error_callback(int error, const char* description) {
    debug.gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

float points[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
};

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouseIn.xPos = xpos;
    mouseIn.yPos = ypos;
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
    
    if (mouseIn.init(shader.window) == 1)
        return 1;
    

    bool init = false;
    
    while(!glfwWindowShouldClose(shader.window)) {
        if (GLFW_PRESS == glfwGetKey(shader.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(shader.window, 1);
        }
        
        glfwSetCursorPosCallback(shader.window, cursor_position_callback);
        
        // Get a handle for our "myTextureSampler" uniform
        GLuint TextureID  = glGetUniformLocation(shader.shader_programme, "myTextureSampler");
        
        GLuint Texture = texture.load();
        
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
        
        
        if (!init)
        {
            init = true;
            debug.gl_log("\nInitialised Successfully. %s %s \n", __DATE__, __TIME__);
        }
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

