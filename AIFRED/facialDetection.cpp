//
//  facialDetection.cpp
//  AIFRED
//
//  Created by Chaidhat Chaimongkol on 09/05/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "facialDetection.hpp"

using namespace facialDetection;

float** facialDetection::process(float **pixels)
{
    
    return pixels;
}

// width 0, height 0 is a 1x1 box
int V_Texture::sum (int x, int y, int width, int height, int **greyPixels)
{
    return (greyPixels[x+width][y+height] - greyPixels[x+width][y-1] - greyPixels[x-1][y+width] + greyPixels[x-1][y-1]);
}
