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
#include <stdlib.h>

namespace AIFRED
{
    namespace FacialDetection
    {
        
        class GreyImage
        {
            int sum(int x, int y, int width, int height);
            
        public:
            const int xs, ys;
            u_int8_t** greyMap;
            int** integralImage;
            
            void process();
            void makeIntegralImage();
            
            GreyImage(int ixs, int iys);
            ~GreyImage();
            
            friend struct Classifiers;
        };
        
        struct Classifiers
        {
            static bool A (int x, int y, int width, int height, int threshold, GreyImage& image);
            static bool B (int x, int y, int width, int height, int threshold, GreyImage& image);
            static bool C (int x, int y, int width, int height, int threshold, GreyImage& image);
            static bool D (int x, int y, int width, int height, int threshold, GreyImage& image);
        };
    }
}

#endif /* facialDetection_hpp */
