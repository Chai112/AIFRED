//
//  playerPhysics.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef playerPhysics_hpp
#define playerPhysics_hpp

#include <stdio.h>

#include <glm/gtc/matrix_transform.hpp>

class G_PlayerPhysics
{
public:
    glm::vec3 playerTransform;
    glm::vec3 playerVelocity;
    glm::vec3 playerAccel;
    
    void update ();
};

#endif /* playerPhysics_hpp */
