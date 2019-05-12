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

namespace AIFRED
{
    namespace FacialDetection
    {
        void init();
        u_int8_t** process(u_int8_t **pixels);
        
        void integralImage(u_int8_t **pixels);
        int sum(int x, int y, int width, int height);
        
        class classifiers
        {
        public:
            bool A (int x, int y, int width, int height, float threshold);
            bool B (int x, int y, int width, int height, float threshold);
            bool C (int x, int y, int width, int height, float threshold);
            bool D (int x, int y, int width, int height, float threshold);
        };
    }
}

#endif /* facialDetection_hpp */
