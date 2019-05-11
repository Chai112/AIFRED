//
//  facialDetection.cpp
//  AIFRED
//
//  Created by Chaidhat Chaimongkol on 09/05/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include "facialDetection.hpp"
#include "texture.hpp"

#include "texture.hpp"


using namespace AIFRED::facialDetection;
using namespace Render::Texture;

static u_int8_t *pixelsG[Render::Texture::xs];

void AIFRED::facialDetection::init()
{
    for (int i=0; i<Render::Texture::xs; i++)
        pixelsG[i] = (u_int8_t *)malloc(Render::Texture::ys * sizeof(u_int8_t));
}

u_int8_t** AIFRED::facialDetection::process(u_int8_t **pixels)
{
    
    for (int x = 0; x < Render::Texture::xs; x++)
    {
        for (int y = 0; y < Render::Texture::ys; y++)
        {
            // (note x = 1 so no conflict)
            if (x > 0 && y > 0)
            {
                pixelsG[x][y] = pixels[x - 1][y] + pixels[x][y - 1] - pixels[x - 1][y - 1] + pixels[x][y];
            }
        }
    }
    printf("%d\n", AIFRED::facialDetection::sum(1,1,20,20));
    return pixels;
}

// width 0, height 0 is a 1x1 box
int AIFRED::facialDetection::sum (int x, int y, int width, int height)
{
    return (pixelsG[x+width][y+height] - pixelsG[x+width][y-1] - pixelsG[x-1][y+width] + pixelsG[x-1][y-1]);
}
