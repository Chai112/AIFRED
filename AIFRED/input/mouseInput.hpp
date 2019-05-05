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

class MouseIn
{
    
public:
    double xPos;
    double yPos;
    
    /*xMouse = xpos / -100;
     //if (sinf(((-yMouse-4.5)/4.5)*180*0.01745329252) > -0.99)
     //yMo += 1;
     if (ypos - yMoo < yMo)
     {
     yMoo = ypos - yMo;
     yMouse = ((ypos - yMoo) / -100);
     }
     else
     {
     if ((cosf(((-(ypos - yMoo) / -100)-4.5)/4.5)*180*0.01745329252) > 0)
     {
     yMo = -99999;
     yMouse = ((ypos - yMoo) / -100);
     }
     else
     {
     if (yMo == -99999)
     yMo = ypos - yMoo;
     }
     }*/
    
    float x ();
    float y ();
    float z ();
    
    int init (GLFWwindow* window);
    
private:
    float yl ();
    float zl ();
    
    double xMouse ();
    double yMouse ();
    double yMo = 0;
    double yMoo = 0;
};

#endif /* mouseInput_hpp */
