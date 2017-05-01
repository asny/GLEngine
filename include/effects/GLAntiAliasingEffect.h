//
//  Created by Asger Nyman Christiansen on 30/04/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLPostEffect.h"

namespace gle
{
    class GLAntiAliasingEffect : public GLPostEffect
    {
    public:
        
        int screen_width = 1;
        int screen_height = 1;
        
        GLAntiAliasingEffect() : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/anti_aliasing_effect.frag")
        {
            
        }
        
        void apply(const GLColorRenderTarget& source_render_target, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            source_render_target.bind_color_texture_for_reading(0, 0);
            GLUniform::use(shader, "colorMap", 0);
            GLUniform::use(shader, "step_x", 1.f/static_cast<float>(screen_width));
            GLUniform::use(shader, "step_y", 1.f/static_cast<float>(screen_height));
            
            draw();
        }
    };
}
