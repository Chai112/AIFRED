//
//  facialDetection.hpp
//  AIFRED
//
//  Created by Chaidhat Chaimongkol on 09/05/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//
//  Viola-Jones paper
//  AND
//  https://medium.com/datadriveninvestor/understanding-and-implementing-the-viola-jones-image-classification-algorithm-85621f7fe20b
//

#ifndef facialDetection_hpp
#define facialDetection_hpp

#include <stdio.h>
#include <stdlib.h>

#include "debug.hpp"

namespace AIFRED
{
    namespace FacialDetection
    {
        typedef float Percent;
        
        struct Feature
        {
            
            
            Percent faceHaarAverage;
            Percent faceCorrect;
            
            Percent nonFaceHaarAverage;
            Percent nonFaceCorrect;
            
            float faceHaarTotal = 0;
            float nonFaceHaarTotal = 0;
            
            int x = 0;
            int y = 0;
            
            int w = 0;
            int h = 0;
            
            int type = 0;
            
        };
        
        struct FeatureImage
        {
            Feature *featuresSorted;
        };
        
        class GreyImage
        {
            float sum(int x, int y, int width, int height);
            float abs(float in);
            void sort(Feature *features, int length);
			void prune(Feature *features, int length, int &out, Percent threshold);
			void draw(int hFAIndex);
            
        public:
            const int xs, ys;
            int mxs, mys; // modifiable for image to be cropped
            u_int8_t** greyMap;
            u_int64_t** integralImage;
            Feature* imageFeatures;
            FeatureImage imageFeaturesEval;
            
            void process();
            void makeIntegralImage();
            void evaluateImage(int iteration, bool b_sort);
            
            GreyImage(int ixs, int iys);
            void initSetFeatures(int imxs, int imys);
            ~GreyImage();
            
            friend struct Classifiers;
        };
        
        struct Classifiers
        {
            static bool A (int x, int y, int width, int height, int threshold, GreyImage& image);
            static bool B (int x, int y, int width, int height, int threshold, GreyImage& image);
            static bool C (int x, int y, int width, int height, int threshold, GreyImage& image);
            static bool D (int x, int y, int width, int height, int threshold, GreyImage& image);
            
            static float A (int x, int y, int width, int height, GreyImage& image);
            static float B (int x, int y, int width, int height, GreyImage& image);
            static float C (int x, int y, int width, int height, GreyImage& image);
            static float D (int x, int y, int width, int height, GreyImage& image);
        };
    }
}

#endif /* facialDetection_hpp */
