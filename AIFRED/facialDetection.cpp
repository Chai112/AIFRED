//
//  facialDetection.cpp
//  AIFRED
//
//  Created by Chaidhat Chaimongkol on 09/05/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "facialDetection.hpp"

using namespace facialDetection;

u_int8_t **pixelsG;

u_int8_t** facialDetection::process(u_int8_t **pixels)
{
    pixelsG = pixels;
    //printf("%d\n", facialDetection::sum(0,0,20,20));
    return pixels;
}

// width 0, height 0 is a 1x1 box
int facialDetection::sum (int x, int y, int width, int height)
{
    return (pixelsG[x+width][y+height] - pixelsG[x+width][y-1] - pixelsG[x-1][y+width] + pixelsG[x-1][y-1]);
}
