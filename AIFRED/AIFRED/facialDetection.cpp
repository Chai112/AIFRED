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
int outLength = 0;


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
            static Feature ievalFeatures[PNG_DIMENSION * PNG_DIMENSION * PNG_DIMENSION];
            for (int x=0; x<xs; x++)
            {
                igreyMap[x] = (u_int8_t *)malloc(ys * sizeof(u_int8_t));
                iintegralImage[x] = (u_int64_t *)malloc(ys * sizeof(u_int64_t));
            }
            
            // assign
            greyMap = igreyMap;
            integralImage = iintegralImage;
            imageFeatures = ievalFeatures;
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
		
		
		
        
        void GreyImage::initSetFeatures(int imxs, int imys)
        {
            mxs = imxs;
            mys = imys;
            totalClassiferCount = 0;
            // allocate all evaluations
            const int xIncrement = 2;
            const int yIncrement = 2;
            const int wIncrement = 4;
            const int hIncrement = 4;
            
            // uses mxs and mys as image can be cropped.
            for (int x = 4; x < mxs - xIncrement - 1; x += xIncrement)
            {
                for (int y = 4; y < mys - yIncrement - 1; y += yIncrement)
                {
                    for (int w = 2; w < (mxs - x) / 3; w += wIncrement)
                    {
                        for (int h = 2; h < (mys - y) / 3; h += hIncrement)
                        {
                            for (int i = 0; i < 4; i++)
                            {
                                imageFeatures[totalClassiferCount].x = x;
                                imageFeatures[totalClassiferCount].y = y;
                                imageFeatures[totalClassiferCount].w = w;
                                imageFeatures[totalClassiferCount].h = h;
                                
                                imageFeatures[totalClassiferCount].type = totalClassiferCount % 4 + 1;
                                
                                totalClassiferCount++;
                                
                                greyMap[x][y] = 255;
                            }
                        }
                    }
                }
            }
        }

        // runtime processing
        void GreyImage::process()
        {
            makeIntegralImage();
            Classifiers cl;
            
            // set all evaluations
            for (int i = 0; i < totalClassiferCount; i += 4)
            {
                Feature* e = &imageFeatures[i];
                e->faceHaarTotal += cl.A(e->x, e->y, e->w, e->h, *this);
                
                e = &imageFeatures[i + 1];
                e->faceHaarTotal += cl.B(e->x, e->y, e->w, e->h, *this);
                
                e = &imageFeatures[i + 2];
                if (e->w % 3 == 0 && e->h % 3 == 0)
                    e->faceHaarTotal += cl.C(e->x, e->y, e->w, e->h, *this);
                
                e = &imageFeatures[i + 3];
                e->faceHaarTotal += cl.D(e->x, e->y, e->w, e->h, *this);
            }
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
                        if (integralImage[x][y] >= ULLONG_MAX - 1) // is it overloading?
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
		
		// does evaluation of image classifiers
        void GreyImage::evaluateImage (int iteration, bool b_sort)
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
                Feature *e = &imageFeatures[i];
                
                // this is wrong
                e->faceHaarAverage = e->faceHaarTotal / iteration;
				e->nonFaceHaarAverage = e->nonFaceHaarTotal / iteration;
                
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
            
            imageFeaturesEval.featuresSorted = imageFeatures;
			
			outLength = 0;
			static Feature outFeatures[PNG_DIMENSION * PNG_DIMENSION * PNG_DIMENSION];
			prune(imageFeaturesEval.featuresSorted, totalClassiferCount, outFeatures, outLength, 0.5f);
			printf("%d\n", outLength);
			if (b_sort)
			{
            	imageFeaturesEval.featuresSorted = sort(outFeatures, outLength);
				for (int i = 0; i < outLength; i++) draw(imageFeaturesEval.featuresSorted[i]);
			}
			else
			{
				draw(imageFeaturesEval.featuresSorted[hFAIndex]);
			}
			//printf("asd %d", totalClassiferCount);
			
        }
		
		
		
		Percent GreyImage::evaluate()
		{
			Percent j = 0;
			for (int i = 0; i < outLength; i++)
			{
				Feature f = imageFeaturesEval.featuresSorted[i];
				Classifiers cl;
				if (f.type == 1)
					j += cl.A(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
			
				if (f.type == 2)
					j += cl.B(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
			
				if (f.type == 3)
					j += cl.C(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
			
				if (f.type == 4)
					j += cl.D(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
			}
			printf("eval: %f\n", j);
			return j;
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
        
        float GreyImage::abs (float in)
        {
            if (in < 0)
                return (in * -1);
            
            return in;
        }
        
        Feature* GreyImage::sort(Feature *features, int length)
        {
            bool repeat = true;
            int j = 0;
			while (repeat)
            {
                repeat = false;
                for (int i = 1; i < length; i++)
                {
					// largest number goes up down in index, so [0] is largest
                    float *f = &features[i].faceHaarAverage;
                    float *fp = &features[i - 1].faceHaarAverage;
					if (*f != 0)
					{
						if (*f > *fp)
						{
							repeat = true;
							float ft = *f;
							*f = *fp;
							*fp = ft;
						}
					}
					else
					{
						i++;
					}
                }
                j++;
            }
			printf("a %f\n", features[0].faceHaarAverage);
			return features;
        }
		
		void GreyImage::prune(Feature *features, int length, Feature *outFeatures, int &outLength, Percent threshold)
		{
			// find avg
			float highest = 0;
			float lowest = 0;
			float j = 0;
			for (int i = 0; i < length; i++)
			{
				float *f = &features[i].faceHaarAverage;
				j += *f;
				if (*f > highest)
					highest = *f;
				if (*f < lowest)
					lowest = *f;
			}
			//float avg = GreyImage::abs(j / length);
			highest *= threshold;
			lowest *= threshold;
			
			// prune
			outLength = 0;
			for (int i = 0; i < length; i++)
			{
				float *f = &features[i].faceHaarAverage;
				if (*f < lowest || *f > highest)
				{
					outFeatures[outLength] = features[i];
					outLength++;
				}
			}
		}
		
		void GreyImage::draw (Feature target)
		{
			greyMap[target.x][target.y] = 255;
			if (target.type == 1)
			{
				for (int w = 0; w < target.w * 2; w++)
				{
					greyMap[target.x + w][target.y] = 255;
				}
				for (int w = 0; w < target.h; w++)
				{
					greyMap[target.x][target.y + w] = 255;
				}
			}
			if (target.type == 2)
			{
				for (int w = 0; w < target.w; w++)
				{
					greyMap[target.x + w][target.y] = 255;
				}
				for (int w = 0; w < target.h * 2; w++)
				{
					greyMap[target.x][target.y + w] = 255;
				}
			}
			if (target.type == 3)
			{
				for (int w = 0; w < target.w * 3; w++)
				{
					if ((target.x + w) % 2 == 0)
						greyMap[target.x + w][target.y] = 255;
				}
				for (int w = 0; w < target.h; w++)
				{
					greyMap[target.x][target.y + w] = 255;
				}
			}
			if (target.type == 4)
			{
				for (int w = 0; w < target.w * 2; w++)
				{
					greyMap[target.x + w][target.y] = 255;
				}
				for (int w = 0; w < target.h * 2; w++)
				{
					greyMap[target.x][target.y + w] = 255;
				}
			}
		}
        
		
		// classifiers
		
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
