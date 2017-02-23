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
        std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> uv_coordinates;
        std::shared_ptr<float> time;
    public:
        GLFogEffect(std::shared_ptr<GLTexture> _noise_texture, std::shared_ptr<float> _time)
        : GLPostEffect("../GLEngine/shaders/fog_effect.vert",  "../GLEngine/shaders/fog_effect.frag"), time(_time), noise_texture(_noise_texture)
        {
            
        }
        
        void apply(const GLRenderTarget& source_render_target, float z_near, float z_far)
        {
            source_render_target.bind_depth_texture_for_reading(1);
            noise_texture->use(2);
            
            GLUniform::use(shader, "depthMap", 1);
            GLUniform::use(shader, "noiseTexture", 2);
            GLUniform::use(shader, "time", *time);
            GLUniform::use(shader, "zNear", z_near);
            GLUniform::use(shader, "zFar", z_far);
            
            GLPostEffect::apply(source_render_target, z_near, z_far);
        }
    };
}
