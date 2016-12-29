//
//  Created by Asger Nyman Christiansen on 29/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    class GLLight : public GLMaterial
    {
    protected:
        GLLight(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename)
        {
            shader = GLShader::create_or_get(vertex_shader_filename, fragment_shader_filename);
            
            use_uniform("screenSize", glm::vec2(2400, 1400));
            
            use_uniform_int("positionMap", 0);
            use_uniform_int("colorMap", 1);
            use_uniform_int("normalMap", 2);
        }
        
    public:
        
    };
    
    class GLDirectionalLight : public GLLight
    {
    public:
        
        GLDirectionalLight(const glm::vec3& direction) :
            GLLight("../GLEngine/shaders/light_pass.vert",  "../GLEngine/shaders/light_pass.frag")
        {
            
            use_uniform("lightDirection", direction);
        }
    };
}
