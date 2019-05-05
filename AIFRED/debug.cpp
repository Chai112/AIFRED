//
//  debug.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "debug.hpp"
#include <GLFW/glfw3.h> // GLFW helper library
#include <time.h>
#include <stdarg.h>

#define GL_LOG_FILE "/Users/chaidhatchaimongkol/Documents/ESC/ESC/log/gl.log"

bool Debug::restart_gl_log() {
    FILE* file = fopen(GL_LOG_FILE, "w");
    if(!file) {
        fprintf(stderr,
                "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
                GL_LOG_FILE);
        return false;
    }
    else
    {
    }
    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
    fclose(file);
    return true;
}

bool Debug::gl_log(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if(!file) {
        fprintf(
                stderr,
                "ERROR: could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE
                );
        return false;
    }
    printf(message, argptr);
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

bool Debug::gl_log_err(const char* message, ...) {
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if(!file) {
        fprintf(stderr,
                "ERROR: could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}
