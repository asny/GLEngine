//
//  Created by Asger Nyman Christiansen on 22/02/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLPostEffect.h"

namespace gle
{
    class GLFogEffect : public GLPostEffect
    {
        std::shared_ptr<GLTexture> noise_texture;
    public:
        glm::vec3 color = glm::vec3(0.8, 0.8, 0.8);
        float density = 0.2;
        float min_visibility = 0.05;
        
        GLFogEffect(std::shared_ptr<GLTexture> _noise_texture)
        : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/fog_effect.frag"), noise_texture(_noise_texture)
        {
            
        }
        
        void apply(const GLRenderTarget& source_render_target1, const GLRenderTarget& source_render_target2, float z_near, float z_far)
        {
            noise_texture->use(4);
            GLUniform::use(shader, "noiseTexture", 4);
            
            GLUniform::use(shader, "fogColor", color);
            GLUniform::use(shader, "fogDensity", density);
            GLUniform::use(shader, "minVisibility", min_visibility);
            GLUniform::use(shader, "time", time());
            
            GLUniform::use(shader, "zNear", z_near);
            GLUniform::use(shader, "zFar", z_far);
            
            GLPostEffect::apply(source_render_target1, source_render_target2, z_near, z_far);
        }
    };
}
