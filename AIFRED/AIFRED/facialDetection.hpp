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
        
        class gImage
        {
            int sum(int x, int y, int width, int height);
            
        public:
            const int xs, ys;
            u_int8_t** greyMap;
            int** integralImage;
            
            void process();
            void makeIntegralImage();
            
            gImage(int ixs, int iys);
            ~gImage();
            
            friend struct classifiers;
        };
        
        struct classifiers
        {
            static bool A (int x, int y, int width, int height, int threshold, gImage& image);
            static bool B (int x, int y, int width, int height, int threshold, gImage& image);
            static bool C (int x, int y, int width, int height, int threshold, gImage& image);
            static bool D (int x, int y, int width, int height, int threshold, gImage& image);
        };
    }
}

#endif /* facialDetection_hpp */
