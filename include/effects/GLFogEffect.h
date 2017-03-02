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
        
        void apply(const GLRenderTarget& source_render_target, float z_near, float z_far, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection)
        {
            noise_texture->use(0);
            GLUniform::use(shader, "noiseTexture", 0);
            
//            source_render_target.bind_color_texture_for_reading(1, 1);
//            GLUniform::use(shader, "positionMap", 1);
            
            source_render_target.bind_depth_texture_for_reading(1);
            GLUniform::use(shader, "depthMap", 1);
            
            GLUniform::use(shader, "fogColor", color);
            GLUniform::use(shader, "fogDensity", density);
            GLUniform::use(shader, "minVisibility", min_visibility);
            GLUniform::use(shader, "time", time());
            
            GLUniform::use(shader, "zNear", z_near);
            GLUniform::use(shader, "zFar", z_far);
            
            draw();
        }
    };
}
