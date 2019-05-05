//
//  debug.hpp
//  ESC
//
//  Created by Chaidhat Chaimongkol on 27/04/2019.
//  Copyright © 2019 Chai112. All rights reserved.
//

#ifndef debug_hpp
#define debug_hpp

#include <stdio.h>

class Debug
{
public:
    bool restart_gl_log();
    bool gl_log(const char* message, ...);
    bool gl_log_err(const char* message, ...);
};

#endif /* debug_hpp */
