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
        
        void apply(const GLColorRenderTarget& source_render_target, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            source_render_target.bind_color_texture_for_reading(0, 0);
            GLUniform::use(shader, "colorMap", 0);
            
            source_render_target.bind_depth_texture_for_reading(1);
            GLUniform::use(shader, "depthMap", 1);
            
            GLState::depth_write(true);
            GLState::depth_test(true);
            
            GLObject::draw_full_screen_quad(shader);
        }
    };
}

