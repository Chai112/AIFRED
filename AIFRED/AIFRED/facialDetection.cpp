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
#include "debug.hpp"

#define PNG_DIMENSION 128


using namespace AIFRED::FacialDetection;
using namespace Render::Texture;

int b = 0;

namespace AIFRED
{
    namespace FacialDetection
    {
        
        // GreyImage Constructor
        GreyImage::GreyImage(int ixs, int iys) : xs(ixs), ys(iys)
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
        
        // GreyImage Destructor
        GreyImage::~GreyImage()
        {
            for (int x=0; x<xs; x++)
            {
                delete greyMap[x];
                delete integralImage[x];
            }
        }

        // runtime processing
        void GreyImage::process()
        {
            
            makeIntegralImage();
            Classifiers cl;
            float a = 0;
            if (b == 0)
            {
            for (int x = 4; x < PNG_DIMENSION - 4; x += 4)
            {
                for (int y = 4; y < PNG_DIMENSION - 4; y += 4)
                {
                    for (int w = 2; w < PNG_DIMENSION - x - 5; w += 4)
                    {
                        for (int h = 2; h < PNG_DIMENSION - y - 5; h += 4)
                        {
                            a = cl.A(x, y, w, h, *this);
                            //printf("%f %d\n", a, b);
                            b += 1;
                            //printf("b: %f", cl.B(x, y, w, h, *this));
                            if (w % 3 == 0 && h % 3 == 0)
                            {
                                //printf("c: %f", cl.C(x, y, w, h, *this));
                            }
                            //printf("d: %f", cl.D(x, y, w, h, *this));
                        }
                    }
                }
            }
            }
            b += 1;
        }

        // creates integral image and assigns integral image.
        void GreyImage::makeIntegralImage()
        {
            // integral image
            for (int x = 0; x < xs; x++)
            {
                for (int y = 0; y < ys; y++)
                {
                    // (note x = 1 so no conflict)
                    if (x > 0 && y > 0)
                    {
                        integralImage[x][y] = integralImage[x - 1][y] + integralImage[x][y - 1] - integralImage[x - 1][y - 1] + greyMap[x][y];
                    }
                }
            }
        }
        
        Sum GreyImage::sumImage ()
        {
            Sum s;
            s.average = 2.f;
            return s;
        }

        // width 0, height 0 is a 1x1 box
        float GreyImage::sum (int x, int y, int width, int height)
        {
            width -= 1;
            height -= 1;
            return ((float)(integralImage[x+width][y+height] - integralImage[x+width][y-1] - integralImage[x-1][y+width] + integralImage[x-1][y-1]) / ((width + 1) * (height + 1)));
        }
        
        
            
        bool Classifiers::A (int x, int y, int width, int height, int threshold, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            width /= 2;
            return (threshold < (image.sum(x, y, width, height) - image.sum(x + width, y, width, height)));
            
        }
        
        bool Classifiers::B (int x, int y, int width, int height, int threshold, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            height /= 2;
            return (threshold < (image.sum(x, y, width, height) - image.sum(x, y + height, width, height)));
            
        }
        
        bool Classifiers::C (int x, int y, int width, int height, int threshold, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            width /= 3;
            return (threshold < ((image.sum(x, y, width, height) + image.sum(x + (width * 2), y, width, height)) - (image.sum(x + width, y, width, height) * 2)));
            
        }
        
        bool Classifiers::D (int x, int y, int width, int height, int threshold, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            height /= 2;
            width /= 2;
            return (threshold < ((image.sum(x, y, width, height) + image.sum(x + width, y + height, width, height)) - (image.sum(x + width, y, width, height) + image.sum(x, y + height, width, height))));
            
        }
        
        float Classifiers::A (int x, int y, int width, int height, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            width /= 2;
            return (image.sum(x, y, width, height) - image.sum(x + width, y, width, height));
            
        }
        
        float Classifiers::B (int x, int y, int width, int height, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            height /= 2;
            return (image.sum(x, y, width, height) - image.sum(x, y + height, width, height));
            
        }
        
        float Classifiers::C (int x, int y, int width, int height, GreyImage& image)
        {
            if (width % 3 >= 1 || height % 3 >= 1) // accept three numbers
                abort();
            width /= 3;
            return ((image.sum(x, y, width, height) + image.sum(x + (width * 2), y, width, height)) - (image.sum(x + width, y, width, height) * 2));
            
        }
        
        float Classifiers::D (int x, int y, int width, int height, GreyImage& image)
        {
            if (width % 2 == 1 || height % 2 == 1) // accept even numbers
                abort();
            height /= 2;
            width /= 2;
            return ((image.sum(x, y, width, height) + image.sum(x + width, y + height, width, height)) - (image.sum(x + width, y, width, height) + image.sum(x, y + height, width, height)));
            
        }
    }
}
