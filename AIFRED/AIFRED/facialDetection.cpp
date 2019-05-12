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


using namespace AIFRED::FacialDetection;
using namespace Render::Texture;

static u_int8_t *pixelsG[Render::Texture::xs];

namespace AIFRED
{
    namespace FacialDetection
    {
        void init()
        {
            for (int i=0; i<Render::Texture::xs; i++)
                pixelsG[i] = (u_int8_t *)malloc(Render::Texture::ys * sizeof(u_int8_t));
        }
        
        gImage::gImage (int ixs, int iys) : xs(ixs), ys(iys)
        {
            static u_int8_t* igreyMap[128];
            for (int x=0; x<xs; x++)
            {
                igreyMap[x] = (u_int8_t *)malloc(ys * sizeof(u_int8_t));
                for (int y=0; y<ys; y++)
                    igreyMap[x][y] = 0;
            }
            greyMap = igreyMap;
            printf("init %d", greyMap[0][0]);
        }

        void gImage::process()
        {
            //greyMap = makeIntegralImage (greyMap);
            printf("%d\n", sum(1,1,20,20));
        }

        u_int8_t** gImage::makeIntegralImage (u_int8_t **pixels)
        {
            // integral image
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
            
            return pixelsG;
        }

        // width 0, height 0 is a 1x1 box
        int sum (int x, int y, int width, int height)
        {
            return (pixelsG[x+width][y+height] - pixelsG[x+width][y-1] - pixelsG[x-1][y+width] + pixelsG[x-1][y-1]);
        }
        
        namespace classifiers
        {
            
            bool A (int x, int y, int width, int height, int threshold)
            {
                width /= 2;
                return (threshold < (sum(x, y, width, height) - sum(x + width, y, width, height))) ? true : false;
                
            }
            
            bool B (int x, int y, int width, int height, int threshold)
            {
                height /= 2;
                return (threshold < (sum(x, y, width, height) - sum(x, y + height, width, height))) ? true : false;
                
            }
            
            bool C (int x, int y, int width, int height, int threshold)
            {
                width /= 3;
                return (threshold < ((sum(x, y, width, height) + sum(x + (width * 2), y, width, height)) - sum(x + width, y, width, height))) ? true : false;
                
            }
            
            bool D (int x, int y, int width, int height, int threshold)
            {
                height /= 2;
                width /= 2;
                return (threshold < ((sum(x, y, width, height) + sum(x + width, y + height, width, height)) - (sum(x + width, y, width, height) + sum(x, y + height, width, height)))) ? true : false;
                
            }
        }
    }
}
