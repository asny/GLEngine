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
        float radius;
        std::unique_ptr<GLTexture2D> noise_texture;
        std::vector<glm::vec3> samples;
        const int noise_size = 4;
        const int sample_size = 16;
        glm::mat4 bias_matrix = glm::mat4(0.5, 0.0, 0.0, 0.0,
                                          0.0, 0.5, 0.0, 0.0,
                                          0.0, 0.0, 0.5, 0.0,
                                          0.5, 0.5, 0.5, 1.0);
    public:
        
        GLAmbientOcclusionEffect(float _radius = 1.f)
            : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/ssao_effect.frag"), radius(_radius)
        {
            create_sample_kernel();
            create_noise_texture();
        }
        
        void apply(const GLColorRenderTarget& source_render_target, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            noise_texture->use(0);
            GLUniform::use(shader, "noiseTexture", 0);
            
            source_render_target.bind_color_texture_for_reading(1, 1);
            GLUniform::use(shader, "positionMap", 1);
            
            source_render_target.bind_color_texture_for_reading(2, 2);
            GLUniform::use(shader, "normalMap", 2);
            
            GLUniform::use(shader, "samples", samples[0], sample_size);
            GLUniform::use(shader, "sampleSize", sample_size);
            GLUniform::use(shader, "noiseSize", noise_size);
            
            GLUniform::use(shader, "VPBMatrix", bias_matrix * projection * view);
            GLUniform::use(shader, "eyePosition", camera_position);
            GLUniform::use(shader, "radius", radius);
            
            draw();
        }
        
    private:
        
        void create_sample_kernel()
        {
            for (int i = 0; i < sample_size; ++i)
            {
                float radius = random(0., 1.);
                double theta = random(0., 2. * M_PI);
                double phi = random(0., 0.5 * M_PI);
                samples.push_back(radius * radius * glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
            }
        }
        
        void create_noise_texture()
        {
            auto noise = std::vector<float>(noise_size * noise_size * 3);
            for (int i = 0; i < noise_size * noise_size; ++i)
            {
                double angle = random(0., 2. * M_PI);
                auto n = glm::normalize(glm::vec3(cos(angle), sin(angle), 0.f));
                noise[i*3] = n.x;
                noise[i*3+1] = n.y;
                noise[i*3+2] = n.z;
            }
            noise_texture = std::unique_ptr<GLTexture2D>(new GLTexture2D(&noise[0], noise_size, noise_size));
        }
    };
}
