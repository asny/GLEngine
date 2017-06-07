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
            input.geometry_pass_render_target->bind_color_texture_for_reading(0, 0);
            GLUniform::use(shader, "colorMap", 0);
            
            input.geometry_pass_render_target->bind_color_texture_for_reading(1, 1);
            GLUniform::use(shader, "positionMap", 1);
            
            input.geometry_pass_render_target->bind_color_texture_for_reading(2, 2);
            GLUniform::use(shader, "normalMap", 2);
            
            input.geometry_pass_render_target->bind_depth_texture_for_reading(3);
            GLUniform::use(shader, "depthMap", 3);
            
            GLUniform::use(shader, "type", type);
            
            draw();
        }
    };
}
