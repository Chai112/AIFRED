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

namespace Input
{
    double Mouse::xMouse() const {   return xPos / -100;    }

    double Mouse::yMouse() const {   return yPos / -100;    }
    
    // public coords
    float Mouse::x() const {   return sinf(((-xMouse()-7)/7)*180*0.01745329252) * zl();  }
    
    float Mouse::y() const {   return yl();   }
    
    float Mouse::z() const {   return cosf(((-xMouse()-7)/7)*180*0.01745329252) * zl();  }
    
    int init (GLFWwindow* window) { //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        return 0;}

    // private yMouse coords
    float Mouse::yl() const {   return sinf(((yMouse()-4.5)/4.5)*180*0.01745329252);     }

    float Mouse::zl() const {   return cosf(((-yMouse()-4.5)/4.5)*180*0.01745329252);    }
}
