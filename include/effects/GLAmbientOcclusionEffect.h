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
        
        void apply(const GLRenderTarget& source_render_target1, const GLRenderTarget& source_render_target2, float z_near, float z_far)
        {
            
            
            GLPostEffect::apply(source_render_target1, source_render_target2, z_near, z_far);
        }
        }
    };
}
