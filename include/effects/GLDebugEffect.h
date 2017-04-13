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
        enum Type {NONE = -1, POSITION = 0, NORMAL = 1, COLOR = 2};
        Type type = NONE;
        
        GLDebugEffect() : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/debug_effect.frag")
        {
            
        }
        
        void apply(const GLColorRenderTarget& source_render_target, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            source_render_target.bind_color_texture_for_reading(0, 0);
            GLUniform::use(shader, "colorMap", 0);
            
            source_render_target.bind_color_texture_for_reading(1, 1);
            GLUniform::use(shader, "positionMap", 1);
            
            source_render_target.bind_color_texture_for_reading(2, 2);
            GLUniform::use(shader, "normalMap", 2);
            
            GLUniform::use(shader, "type", type);
            
            draw();
        }
    };
}
