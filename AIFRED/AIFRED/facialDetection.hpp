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
        
        void init();
        int sum(int x, int y, int width, int height);
        
        class gImage
        {
            u_int8_t** makeIntegralImage(u_int8_t **pixels);
            
        public:
            int a;
            const int xs, ys;
            u_int8_t** greyMap;
            int** integralImage;
            
            void process();
            
            gImage(int ixs, int iys);
        };
        
        namespace classifiers
        {
            bool A (int x, int y, int width, int height, int threshold);
            bool B (int x, int y, int width, int height, int threshold);
            bool C (int x, int y, int width, int height, int threshold);
            bool D (int x, int y, int width, int height, int threshold);
        };
    }
}

#endif /* facialDetection_hpp */
