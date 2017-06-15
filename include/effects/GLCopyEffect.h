//
//  Created by Asger Nyman Christiansen on 07/06/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLPostEffect.h"

namespace gle
{
    class GLCopyEffect : public GLPostEffect
    {
    public:
        
        GLCopyEffect() : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/copy_effect.frag")
        {
            
        }
        
        void apply(const DrawPassInput& input) const
        {
            auto shader = get_shader();
            GLState::depth_write(true);
            GLState::depth_test(true);
            
            input.shaded_color_texture->use(0);
            GLUniform::use(shader, "colorMap", 0);
            
            input.depth_texture->use(1);
            GLUniform::use(shader, "depthMap", 1);
            
            GLObject::draw_full_screen_quad(shader);
        }
    };
}

