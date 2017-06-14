//
//  Created by Asger Nyman Christiansen on 22/03/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLPostEffect.h"

namespace gle
{
    class GLDebugEffect : public GLPostEffect
    {
    public:
        enum Type {NONE = -1, POSITION = 0, NORMAL = 1, COLOR = 2, DEPTH = 3};
        Type type = NONE;
        
        GLDebugEffect() : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/debug_effect.frag")
        {
            
        }
        
        void apply(const DrawPassInput& input) const
        {
            auto shader = get_shader();
            GLState::depth_write(false);
            GLState::depth_test(false);
            
            input.color_texture->use(0);
            GLUniform::use(shader, "colorMap", 0);
            
            input.position_texture->use(1);
            GLUniform::use(shader, "positionMap", 1);
            
            input.normal_texture->use(2);
            GLUniform::use(shader, "normalMap", 2);
            
            input.depth_texture->use(3);
            GLUniform::use(shader, "depthMap", 3);
            
            GLUniform::use(shader, "type", type);
            
            GLObject::draw_full_screen_quad(shader);
        }
    };
}
