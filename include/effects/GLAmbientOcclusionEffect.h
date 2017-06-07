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
        std::unique_ptr<GLTexture2D> blue_noise_texture0, blue_noise_texture1, blue_noise_texture2, blue_noise_texture3;
        std::vector<glm::vec3> samples;
        const int noise_size = 4;
        const int sample_size = 4;
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
            blue_noise_texture0 = std::unique_ptr<GLTexture2D>(new GLTexture2D("../GLEngine/HDR_L_0.png"));
            blue_noise_texture1 = std::unique_ptr<GLTexture2D>(new GLTexture2D("../GLEngine/HDR_L_1.png"));
            blue_noise_texture2 = std::unique_ptr<GLTexture2D>(new GLTexture2D("../GLEngine/HDR_L_2.png"));
            blue_noise_texture3 = std::unique_ptr<GLTexture2D>(new GLTexture2D("../GLEngine/HDR_L_3.png"));
        }
        
        void apply(const DrawPassInput& input) const
        {
            noise_texture->use(0);
            GLUniform::use(shader, "noiseTexture", 0);
            
            input.position_texture->use(1);
            GLUniform::use(shader, "positionMap", 1);
            
            input.normal_texture->use(2);
            GLUniform::use(shader, "normalMap", 2);
            
            blue_noise_texture0->use(3);
            GLUniform::use(shader, "blueNoiseTexture0", 3);
            blue_noise_texture0->use(4);
            GLUniform::use(shader, "blueNoiseTexture1", 4);
            blue_noise_texture0->use(5);
            GLUniform::use(shader, "blueNoiseTexture2", 5);
            blue_noise_texture0->use(6);
            GLUniform::use(shader, "blueNoiseTexture3", 6);
            
            GLUniform::use(shader, "samples", samples[0], sample_size);
            GLUniform::use(shader, "sampleSize", sample_size);
            GLUniform::use(shader, "noiseSize", noise_size);
            
            GLUniform::use(shader, "VPBMatrix", bias_matrix * input.projection * input.view);
            GLUniform::use(shader, "eyePosition", input.camera_position);
            GLUniform::use(shader, "radius", radius);
            
            draw();
        }
        
    private:
        
        void create_sample_kernel()
        {
            for (int i = 0; i < sample_size; ++i)
            {
                double theta = random(0., 2. * M_PI);
                double phi = random(0., 0.5 * M_PI);
                samples.push_back(glm::vec3(cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi)));
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
