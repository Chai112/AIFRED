//
//  facialDetection.hpp
//  AIFRED
//
//  Created by Chaidhat Chaimongkol on 09/05/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef facialDetection_hpp
#define facialDetection_hpp

#include <stdio.h>

namespace facialDetection
{
    const int a = 0;
    float** process(float **pixels);
    int V_Texture::sum (int x, int y, int width, int height, int **greyPixels);
    /*    // processing
     for (int x = 0; x < xs; x++)
     {
     for (int y = 0; y < ys; y++)
     {
     // (note x = 1 so no conflict)
     if (x > 0 && y > 0)
     {
     greyPixels[x][y] = greyPixels[x - 1][y] + greyPixels[x][y - 1] - greyPixels[x - 1][y - 1] + greyPixels[x][y];
     }
     }
     }*/
}

#endif /* facialDetection_hpp */
