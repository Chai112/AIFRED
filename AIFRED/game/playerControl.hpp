//
//  keyboardInput.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef playerControl_hpp
#define playerControl_hpp

#include <stdio.h>

#include "mouseInput.hpp"

#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/gtc/matrix_transform.hpp>


class G_PlayerControl
{
public:
    glm::vec3 accel;
    glm::vec3 velocity;
    
    void update (GLFWwindow* window, MouseIn mouseIn);
    
private:
    short keyPx = 0;
    short keyPy = 0;
};

#endif /* playerControl_hpp */
