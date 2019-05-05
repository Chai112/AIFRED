//
//  mouseInput.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "mouseInput.hpp"

#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/gtc/matrix_transform.hpp>

double MouseIn::xMouse() {   return MouseIn::xPos / -100;    }

double MouseIn::yMouse() {   return MouseIn::yPos / -100;    }


// private yMouse coords
float MouseIn::yl() {   return sinf(((MouseIn::yMouse()-4.5)/4.5)*180*0.01745329252);     }

float MouseIn::zl() {   return cosf(((-MouseIn::yMouse()-4.5)/4.5)*180*0.01745329252);    }


// public coords
float MouseIn::x() {   return sinf(((-MouseIn::xMouse()-7)/7)*180*0.01745329252) * MouseIn::zl();  }

float MouseIn::y() {   return MouseIn::yl();   }

float MouseIn::z() {   return cosf(((-MouseIn::xMouse()-7)/7)*180*0.01745329252) * MouseIn::zl();  }

int MouseIn::init (GLFWwindow* window) { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); return 0;}
