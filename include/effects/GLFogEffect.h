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
    public:
        glm::vec3 color = glm::vec3(0.8, 0.8, 0.8);
        float density = 0.2;
        float no_fog_height = 3;
        float animation = 0.1;
        
        GLFogEffect()
        : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/fog_effect.frag")
        {
            
        }
        
        void apply(const DrawPassInput& input) const
        {
            auto shader = get_shader();
            GLState::depth_write(false);
            GLState::depth_test(false);
            
            input.position_texture->use(0);
            GLUniform::use(shader, "positionMap", 0);
            
            GLUniform::use(shader, "fogColor", color);
            GLUniform::use(shader, "fogDensity", density);
            GLUniform::use(shader, "noFogHeight", no_fog_height);
            GLUniform::use(shader, "animation", animation);
            GLUniform::use(shader, "time", time());
            GLUniform::use(shader, "eyePosition", input.camera_position);
            
            GLObject::draw_full_screen_quad(shader);
        }
    };
}
