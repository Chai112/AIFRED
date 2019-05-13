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

#define PNG_DIMENSION 128


using namespace AIFRED::FacialDetection;
using namespace Render::Texture;

namespace AIFRED
{
    namespace FacialDetection
    {
        
        gImage::gImage (int ixs, int iys) : xs(ixs), ys(iys)
        {
            // make image
            static u_int8_t* igreyMap[PNG_DIMENSION];
            static int* iintegralImage[PNG_DIMENSION];
            for (int x=0; x<xs; x++)
            {
                igreyMap[x] = (u_int8_t *)malloc(ys * sizeof(u_int8_t));
                iintegralImage[x] = (int *)malloc(ys * sizeof(int));
            }
            
            // assign
            greyMap = igreyMap;
            integralImage = iintegralImage;
        }
        
        gImage::~gImage ()
        {
            free(greyMap);
            free(integralImage);
        }

        void gImage::process()
        {
            makeIntegralImage();
            printf("%d\n", sum(1,1,20,20));
        }

        void gImage::makeIntegralImage()
        {
            // integral image
            for (int x = 0; x < xs; x++)
            {
                for (int y = 0; y < ys; y++)
                {
                    // (note x = 1 so no conflict)
                    if (x > 0 && y > 0)
                    {
                        integralImage[x][y] = greyMap[x - 1][y] + greyMap[x][y - 1] - greyMap[x - 1][y - 1] + greyMap[x][y];
                    }
                }
            }
        }

        // width 0, height 0 is a 1x1 box
        int gImage::sum (int x, int y, int width, int height)
        {
            return (integralImage[x+width][y+height] - integralImage[x+width][y-1] - integralImage[x-1][y+width] + integralImage[x-1][y-1]);
        }
            
        bool classifiers::A (int x, int y, int width, int height, int threshold, gImage& image)
        {
            width /= 2;
            return (threshold < (image.sum(x, y, width, height) - image.sum(x + width, y, width, height))) ? true : false;
            
        }
        
        bool classifiers::B (int x, int y, int width, int height, int threshold, gImage& image)
        {
            height /= 2;
            return (threshold < (image.sum(x, y, width, height) - image.sum(x, y + height, width, height))) ? true : false;
            
        }
        
        bool classifiers::C (int x, int y, int width, int height, int threshold, gImage& image)
        {
            width /= 3;
            return (threshold < ((image.sum(x, y, width, height) + image.sum(x + (width * 2), y, width, height)) - image.sum(x + width, y, width, height))) ? true : false;
            
        }
        
        bool classifiers::D (int x, int y, int width, int height, int threshold, gImage& image)
        {
            height /= 2;
            width /= 2;
            return (threshold < ((image.sum(x, y, width, height) + image.sum(x + width, y + height, width, height)) - (image.sum(x + width, y, width, height) + image.sum(x, y + height, width, height)))) ? true : false;
            
        }
    }
}
