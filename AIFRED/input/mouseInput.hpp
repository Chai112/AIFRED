//
//  mouseInput.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef mouseInput_hpp
#define mouseInput_hpp

#include <GLFW/glfw3.h> // GLFW helper library

namespace Input
{
    class Mouse
    {
        
        double xPos = 0;
        double yPos = 0;
        
        float x () const;
        float y () const;
        float z () const;
        
        int init (GLFWwindow* window);
        
        float yl () const;
        float zl () const;
        
        double xMouse () const;
        double yMouse () const;
    };
}

#endif /* mouseInput_hpp */
