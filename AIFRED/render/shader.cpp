//
//  shader.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "shader.hpp"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

int V_Shader::init()
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    float points[] = {
        -1.0f,  -1.0f,  0.0f,
        1.f, -1.0f,  0.0f,
        -1.0f, 1.f,  0.0f,
        -1.0f,  1.f,  0.0f,
        1.f, -1.0f,  0.0f,
        1.f, 1.f,  0.0f
    };
    
    // Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
    static const GLfloat g_uv_buffer_data[] = {
        1.f, 1.f,
        0.0f, 1.f,
        1.f, 0.0f,
        1.f, 0.0f,
        0.0f, 1.f,
        0.0f, 0.0f,
    };
    
    // uncomment these lines if on Apple OS xMouse
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    //const GLFWvidmode* vmode = glfwGetVideoMode(mon);
    GLFWwindow* window = glfwCreateWindow(900, 900, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    V_Shader::window = window;
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    glfwMakeContextCurrent(window);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("\nRenderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), points, GL_STATIC_DRAW);
    V_Shader::vbo = vbo;
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    V_Shader::vao = vao;
    
    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    V_Shader::vuv = uvbuffer;
    
    V_Shader::vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(V_Shader::vs, 1, &(V_Shader::vertex_shader), NULL);
    glCompileShader(V_Shader::vs);
    V_Shader::fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(V_Shader::fs, 1, &(V_Shader::fragment_shader), NULL);
    glCompileShader(V_Shader::fs);
    
    V_Shader::shader_programme = glCreateProgram();
    glAttachShader(V_Shader::shader_programme, V_Shader::fs);
    glAttachShader(V_Shader::shader_programme, V_Shader::vs);
    glLinkProgram(V_Shader::shader_programme);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClearColor(0, 0, 0.1, 1);
    
    return 0;
}

void V_Shader::update ()
{
    GLFWwindow* window = V_Shader::window;
    // uncomment these lines if on Apple OS xMouse
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // wipe the drawing surface clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    //gluPerspective( 65.0, (double)1024/(double)768, 1.0, 60.0 );
    glLoadIdentity();
    glOrtho(0,1024,768,0,100,-100);
    glMatrixMode(GL_MODELVIEW);
    glUseProgram(V_Shader::shader_programme);
    glBindVertexArray(V_Shader::vao);
    
    glDrawArrays(GL_TRIANGLES, 0, 6 * 3);
    // update other events like input handling
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
}
