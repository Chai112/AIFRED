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

int totalClassiferCount = 0;


using namespace AIFRED::FacialDetection;
using namespace Render::Texture;

namespace AIFRED
{
    namespace FacialDetection
    {
        
        // GreyImage Constructor
        GreyImage::GreyImage(int ixs, int iys) : xs(ixs), ys(iys)
        {
            // make image
            static u_int8_t* igreyMap[PNG_DIMENSION];
            static u_int64_t* iintegralImage[PNG_DIMENSION];
            static Evaluation ievalClassifiers[PNG_DIMENSION * PNG_DIMENSION * PNG_DIMENSION];
            for (int x=0; x<xs; x++)
            {
                igreyMap[x] = (u_int8_t *)malloc(ys * sizeof(u_int8_t));
                iintegralImage[x] = (u_int64_t *)malloc(ys * sizeof(u_int64_t));
            }
            
            // assign
            greyMap = igreyMap;
            integralImage = iintegralImage;
            evalClassifiers = ievalClassifiers;
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
            u_int64_t c = 0;
            for (int x = 0; x < PNG_DIMENSION; x++)
            {
                for (int y = 0; y < PNG_DIMENSION; y++)
                {
                    c+=sum(2,2,2,2);
                }
            }
            printf("c: %d\n", c);
            Classifiers cl;
            float a = 0;
            totalClassiferCount = 0;
            // allocate all evaluations
            int xIncrement = 2;
            int yIncrement = 2;
            int wIncrement = 4;
            int hIncrement = 4;
            
            for (int x = 4; x < PNG_DIMENSION - xIncrement - 1; x += xIncrement)
            {
                for (int y = 4; y < PNG_DIMENSION - yIncrement - 1; y += yIncrement)
                {
                    for (int w = 2; w < (PNG_DIMENSION / 3)  - x; w += wIncrement)
                    {
                        for (int h = 2; h < (PNG_DIMENSION / 3)  -  y; h += hIncrement)
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                evalClassifiers[totalClassiferCount].x = x;
                                evalClassifiers[totalClassiferCount].y = y;
                                evalClassifiers[totalClassiferCount].w = w;
                                evalClassifiers[totalClassiferCount].h = h;
                                
                                evalClassifiers[totalClassiferCount].type = totalClassiferCount % 4 + 1;
                                
                                totalClassiferCount++;
                            }
                        }
                    }
                }
            }
            printf("tcc: %d\n", totalClassiferCount);
            
            c = 0;
            // set all evaluations
            for (int i = 0; i < totalClassiferCount; i += 4)
            {
                Evaluation* e = &evalClassifiers[i];
                e->faceHaarTotal = cl.A(e->x, e->y, e->w, e->h, *this);
                c+=e->faceHaarTotal;
                
                e = &evalClassifiers[i + 1];
                e->faceHaarTotal = cl.B(e->x, e->y, e->w, e->h, *this);
                c+=e->faceHaarTotal;
                
                e = &evalClassifiers[i + 2];
                if (e->w % 3 == 0 && e->h % 3 == 0)
                e->faceHaarTotal = cl.C(e->x, e->y, e->w, e->h, *this);
                c+=e->faceHaarTotal;
                
                e = &evalClassifiers[i + 3];
                e->faceHaarTotal = cl.D(e->x, e->y, e->w, e->h, *this);
                c=e->faceHaarTotal;
            }
            
            /*int x = 70;
            int y = 20;
            int w = 2;
            int h = 20;*/
            printf("asd: %f\n", cl.A(4, 4, 32, 2, *this) / 255);
            printf("asd: %f\n", sum(2,1,60,1));
            printf("asd: %f\n", sum(62,1,60,1));
            /*w-=1;
            h-=1;
            u_int64_t ixy = integralImage[x+w][y+h];
            u_int64_t ix = integralImage[x+w][y-1];
            u_int64_t iy = integralImage[x-1][y+h];
            u_int64_t i = integralImage[x-1][y-1];
            printf("asd: %llu\n", ixy - ix - iy + i);
            printf("asd: %llu\n", ix);
            printf("asd: %llu\n", iy);
            printf("asd: %llu\n", i);*/
            
            //return ((float)(integralImage[x+width][y+height] - integralImage[x+width][y-1] - integralImage[x-1][y+width] + integralImage[x-1][y-1]) / ((width + 1) * (height + 1)));
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
                        if (integralImage[x][y] >= ULLONG_MAX - 1)
                            abort();
                    }
                    else
                    {
                        // padding
                        integralImage[x][y] = 0;
                    }
                }
            }
        }
        
        void GreyImage::evaluateImage ()
        {
            // find all classifier averages
            // find best classifier
            float highestFaceAverage = 0;
            unsigned int hFAIndex = 0;
            float highestNonFaceAverage = 0;
            unsigned int hNFAIndex = 0;
            for (int i = 0; i < totalClassiferCount; i++)
            {
                // find all classifier averages
                Evaluation *e = &evalClassifiers[i];
                
                // this is wrong
                e->faceHaarAverage = e->faceHaarTotal / 255;
                //e->faceHaarAverage = e->faceHaarTotal / (2 * totalClassiferCount);
                //e->nonFaceHaarAverage = e->nonFaceHaarTotal / (2 * totalClassiferCount);
                
                // find best classifier
                if (highestFaceAverage < GreyImage::abs(e->faceHaarAverage))
                {
                    highestFaceAverage = GreyImage::abs(e->faceHaarAverage);
                    hFAIndex = i;
                }
                
                if (highestNonFaceAverage < GreyImage::abs(e->nonFaceHaarAverage))
                {
                    highestNonFaceAverage = GreyImage::abs(e->nonFaceHaarAverage);
                    hNFAIndex = i;
                }
            }
            
            evalImage.bestEval = evalClassifiers[hFAIndex];
            printf("beatype %d", evalImage.bestEval.type);
            printf("asdjjk %f", evalImage.bestEval.faceHaarTotal);
        }
        
        float GreyImage::abs (float in)
        {
            if (in < 0)
                return (in * -1);
            
            return in;
        }

        // width 0, height 0 is a 1x1 box
        float GreyImage::sum (int x, int y, int width, int height)
        {
            width -= 1;
            height -= 1;
            u_int64_t ixy = integralImage[x+width][y+height];
            u_int64_t ix = integralImage[x+width][y-1];
            u_int64_t iy = integralImage[x-1][y+height];
            u_int64_t i = integralImage[x-1][y-1];
            width += 1;
            height += 1;
            return ((float)(ixy - ix - iy + i) / (width * height));
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
            return (image.sum(x, y, width, height) - image.sum(x + width, y, width, height));
            
        }
        
        float Classifiers::B (int x, int y, int width, int height, GreyImage& image)
        {
            return (image.sum(x, y, width, height) - image.sum(x, y + height, width, height));
            
        }
        
        float Classifiers::C (int x, int y, int width, int height, GreyImage& image)
        {
            return ((image.sum(x, y, width, height) + image.sum(x + (width * 2), y, width, height)) - (image.sum(x + width, y, width, height) * 2));
            
        }
        
        float Classifiers::D (int x, int y, int width, int height, GreyImage& image)
        {
            return ((image.sum(x, y, width, height) + image.sum(x + width, y + height, width, height)) - (image.sum(x + width, y, width, height) + image.sum(x, y + height, width, height)));
            
        }
    }
}
