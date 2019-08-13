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
		// wrote myself :D no help
		template<typename T>
		void arrayHeapAllocate(T ***var, const int x, const int y)
		{
			T **tempVar = new T*[x];
			for (int i = 0; i < x; i++)
				tempVar[i] = new T[y];
			
			*var = tempVar;

		}
		using namespace Render::Texture;
		FDScanner::FDScanner(Image &image) : Image(image.sizeX, image.sizeY)
		{
			
			// create greyMap as 2D array
			arrayHeapAllocate<colourByte> (&greyMap, sizeX, sizeY);
			// create integral image as 2D array
			arrayHeapAllocate<u_int64_t> (&integralImage, sizeX, sizeY);
		}
		FDScanner::~FDScanner()
		{
			
		}
		
		
			
        // FDSingleScanner Constructor
        FDSingleScanner::FDSingleScanner(int inSizeX, int inSizeY) : sizeX(inSizeX), sizeY(inSizeY), integralImageProvided(false)
        {
			init();
			initSetFeatures(sizeX, sizeY);
        }
		
			// FDSingleScanner Constructor
		FDSingleScanner::FDSingleScanner(int inSizeX, int inSizeY, int cropMxs, int cropMys) : sizeX(inSizeX), sizeY(inSizeY), integralImageProvided(false)
		{
			init();
			initSetFeatures(cropMxs, cropMys);
		}
		
		void FDSingleScanner::init()
		{
			static Feature ievalFeatures[PNG_DIMENSION * PNG_DIMENSION * PNG_DIMENSION];
			
				// create greyMap as 2D array
			
			arrayHeapAllocate<colourByte> (&greyMap, sizeX, sizeY);
				// create integral image as 2D array
			arrayHeapAllocate<u_int64_t> (&integralImage, sizeX, sizeY);
			
			imageFeatures = ievalFeatures;
		}
		
		// FDSingleScanner Destructor
		FDSingleScanner::~FDSingleScanner()
		{
			for (int x=0; x<sizeX; x++)
			{
				delete greyMap[x];
				delete integralImage[x];
			}
			delete greyMap;
			delete integralImage;
		}
		
		
		
        
        void FDSingleScanner::initSetFeatures(int imxs, int imys)
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

        // runtime processing, analyses the features
        void FDSingleScanner::process()
        {
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
		
		// runtime processing
		void FDSingleScanner::process(Render::Texture::colourByte** igreyMap)
		{
			greyMap = igreyMap;
			makeIntegralImage();
			process();
		}
		
		// runtime processing
		void FDSingleScanner::process(u_int64_t ** iintegralImage)
		{
			integralImage = iintegralImage;
			process();
		}
		

        // creates integral image and assigns integral image.
        void FDSingleScanner::makeIntegralImage()
        {
            // integral image
            for (int x = 0; x < sizeX; x++)
            {
                for (int y = 0; y < sizeY; y++)
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
        void FDSingleScanner::evaluateImage (int iteration, bool b_sort)
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
				
                e->faceHaarAverage = e->faceHaarTotal / iteration;
				e->nonFaceHaarAverage = e->nonFaceHaarTotal / iteration;
                
                // find best classifier
                if (highestFaceAverage < FDSingleScanner::abs(e->faceHaarAverage))
                {
                    highestFaceAverage = FDSingleScanner::abs(e->faceHaarAverage);
                    hFAIndex = i;
                }
                
                if (highestNonFaceAverage < FDSingleScanner::abs(e->nonFaceHaarAverage))
                {
                    highestNonFaceAverage = FDSingleScanner::abs(e->nonFaceHaarAverage);
                    hNFAIndex = i;
                }
                
            }
            
            imageFeaturesEval.featuresSorted = imageFeatures;
			
			outLength = 0;
			static Feature outFeatures[PNG_DIMENSION * PNG_DIMENSION * PNG_DIMENSION];
			prune(imageFeaturesEval.featuresSorted, totalClassiferCount, outFeatures, outLength, 0.5f);
			//printf("%d\n", outLength);
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
		
		
		
		Eval FDSingleScanner::evaluate(Render::Texture::colourByte** igreyMap)
		{
			greyMap = igreyMap;
			makeIntegralImage();
			
			for (int i = 0; i < outLength; i++) draw(imageFeaturesEval.featuresSorted[i]);
			
			Percent j = 0;
			float fa = 0;
			Percent threshold = 0.5f;
			for (int i = 0; i < outLength; i++)
			{
				Feature f = imageFeaturesEval.featuresSorted[i];
				Classifiers cl;
				if (f.type == 1)
				{
					float e = cl.A(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
					if (FDSingleScanner::abs(e) < threshold * 128)
					{
						fa++;
						j += e;
					}
				}
			
				if (f.type == 2)
				{
					float e = cl.B(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
					if (FDSingleScanner::abs(e) < threshold * 128)
						{
							fa++;
							j += e;
						}
				}
				
				if (f.type == 3)
				{
					float e = cl.C(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
					if (FDSingleScanner::abs(e) < threshold * 128)
					{fa++;
						j += e;
					}
				}
				
				if (f.type == 4)
				{
					float e = cl.D(f.x, f.y, f.w, f.h, *this) - f.faceHaarAverage;
					if (FDSingleScanner::abs(e) < threshold * 128)
					{fa++;
						j += e;
					}
				}
			}
			//printf("   eval: %f\n", j / outLength);
			//printf("   eval:  %f\n", (fa));
			
			Eval eval;
			eval.evalPerc = j / outLength;
			eval.failPerc = fa;
			
			return eval;
		}
		
		
		
		// width 0, height 0 is a 1x1 box
		float FDSingleScanner::sum (int x, int y, int width, int height)
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
        
        float FDSingleScanner::abs (float in)
        {
            if (in < 0)
                return (in * -1);
            
            return in;
        }
        
        Feature* FDSingleScanner::sort(Feature *features, int length)
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
			return features;
        }
		
		
		void FDSingleScanner::prune(Feature *features, int length, Feature *outFeatures, int &outLength, Percent threshold)
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
			//float avg = FDSingleScanner::abs(j / length);
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
		
		void FDSingleScanner::draw (Feature target)
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
		
		bool Classifiers::A (int x, int y, int width, int height, int threshold, FDSingleScanner& image)
		{
			if (width % 2 == 1 || height % 2 == 1) // accept even numbers
				abort();
			width /= 2;
			return (threshold < (image.sum(x, y, width, height) - image.sum(x + width, y, width, height)));
			
		}
		
		bool Classifiers::B (int x, int y, int width, int height, int threshold, FDSingleScanner& image)
		{
			if (width % 2 == 1 || height % 2 == 1) // accept even numbers
				abort();
			height /= 2;
			return (threshold < (image.sum(x, y, width, height) - image.sum(x, y + height, width, height)));
			
		}
		
		bool Classifiers::C (int x, int y, int width, int height, int threshold, FDSingleScanner& image)
		{
			if (width % 2 == 1 || height % 2 == 1) // accept even numbers
				abort();
			width /= 3;
			return (threshold < ((image.sum(x, y, width, height) + image.sum(x + (width * 2), y, width, height)) - (image.sum(x + width, y, width, height) * 2)));
			
		}
		
		bool Classifiers::D (int x, int y, int width, int height, int threshold, FDSingleScanner& image)
		{
			if (width % 2 == 1 || height % 2 == 1) // accept even numbers
				abort();
			height /= 2;
			width /= 2;
			return (threshold < ((image.sum(x, y, width, height) + image.sum(x + width, y + height, width, height)) - (image.sum(x + width, y, width, height) + image.sum(x, y + height, width, height))));
			
		}
		
		float Classifiers::A (int x, int y, int width, int height, FDSingleScanner& image)
		{
			return (image.sum(x, y, width, height) - image.sum(x + width, y, width, height));
			
		}
		
		float Classifiers::B (int x, int y, int width, int height, FDSingleScanner& image)
		{
			return (image.sum(x, y, width, height) - image.sum(x, y + height, width, height));
			
		}
		
		float Classifiers::C (int x, int y, int width, int height, FDSingleScanner& image)
		{
			return ((image.sum(x, y, width, height) + image.sum(x + (width * 2), y, width, height)) - (image.sum(x + width, y, width, height) * 2));
			
		}
		
		float Classifiers::D (int x, int y, int width, int height, FDSingleScanner& image)
		{
			return ((image.sum(x, y, width, height) + image.sum(x + width, y + height, width, height)) - (image.sum(x + width, y, width, height) + image.sum(x, y + height, width, height)));
			
		}
    }
}
