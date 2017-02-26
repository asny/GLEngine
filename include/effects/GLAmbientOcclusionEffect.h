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
        std::unique_ptr<GLTexture2D> sample_texture, noise_texture;
    public:
        
        GLAmbientOcclusionEffect()
        : GLPostEffect("../GLEngine/shaders/effect.vert",  "../GLEngine/shaders/ssao_effect.frag")
        {
            create_sample_kernel();
            create_noise_texture();
        }
        
        void apply(const GLRenderTarget& source_render_target, float z_near, float z_far)
        {
            sample_texture->use(3);
            noise_texture->use(4);
            
            GLUniform::use(shader, "sampleTexture", 3);
            GLUniform::use(shader, "noiseTexture", 4);
            
            GLPostEffect::apply(source_render_target, z_near, z_far);
        }
        
    private:
        
        void create_sample_kernel()
        {
            const int kernel_size = 4;
            auto kernel = std::vector<float>(kernel_size * kernel_size * 3);
            for (int i = 0; i < kernel_size * kernel_size; ++i)
            {
                auto sample = glm::normalize(glm::vec3(random(-1.0f, 1.0f), random(-1.0f, 1.0f), random(0.0f, 1.0f)));
                sample *= random(0.0f, 1.0f);
                float scale = float(i) / float(kernel_size);
                float t = scale * scale;
                sample *= (1. - t) * 0.1 + t;
                
                kernel[i*3] = sample.x;
                kernel[i*3+1] = sample.y;
                kernel[i*3+2] = sample.z;
            }
            sample_texture = std::unique_ptr<GLTexture2D>(new GLTexture2D(&kernel[0], kernel_size, kernel_size));
        }
        
        void create_noise_texture()
        {
            const int noise_size = 4;
            auto noise = std::vector<float>(noise_size * noise_size * 3);
            for (int i = 0; i < noise_size * noise_size; ++i)
            {
                auto n = normalize(glm::vec3(
                                random(-1.0f, 1.0f),
                                random(-1.0f, 1.0f),
                                0.0f));
                noise[i*3] = n.x;
                noise[i*3+1] = n.y;
                noise[i*3+2] = n.z;
            }
            noise_texture = std::unique_ptr<GLTexture2D>(new GLTexture2D(&noise[0], noise_size, noise_size));
        }
    };
}
