//
//  Created by Asger Nyman Christiansen on 25/02/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLPostEffect.h"

namespace gle
{
    class GLAmbientOcclusionEffect : public GLPostEffect
    {
    public:
        
        GLAmbientOcclusionEffect()
        : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/ssao_effect.frag")
        {
            
        }
        
        void apply(const GLRenderTarget& source_render_target, float z_near, float z_far)
        {
            source_render_target.bind_depth_texture_for_reading(1);
            
            GLUniform::use(shader, "depthMap", 1);
            
            GLPostEffect::apply(source_render_target, z_near, z_far);
        }
    };
}
