//
//  perspective.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef perspective_hpp
#define perspective_hpp

#include <stdio.h>

//#include "shader.hpp" // must be above glfw
#include "mouseInput.hpp"

#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/gtc/matrix_transform.hpp>


class V_Perspective
{
public:
    void update (GLFWwindow* window, MouseIn mouseIn, glm::vec3 playerTransform, GLuint shader_programme);
};

#endif /* perspective_hpp */
