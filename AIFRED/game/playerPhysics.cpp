//
//  playerPhysics.cpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#include "playerPhysics.hpp"

#include <glm/gtc/matrix_transform.hpp>

void G_PlayerPhysics::update ()
{
    G_PlayerPhysics::playerVelocity = G_PlayerPhysics::playerVelocity + (G_PlayerPhysics::playerAccel / glm::vec3(1));

    G_PlayerPhysics::playerTransform = G_PlayerPhysics::playerTransform + (G_PlayerPhysics::playerVelocity / glm::vec3(1000));
}
