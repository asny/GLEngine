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
        
        
    public:
        
    };
    
    
    
    class GLDirectionalLight : public GLLight
    {
    public:
        
        GLDirectionalLight()
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/light_pass.vert",  "../GLEngine/shaders/light_pass.frag");
            
            use_uniform("eyePosition", camera_position);
            use_uniform("lightDirection", glm::vec3(-1., -1., -1.));
            use_uniform("screenSize", glm::vec2(2400, 1400));
            
            use_uniform_int("positionMap", 0);
            use_uniform_int("colorMap", 1);
            use_uniform_int("normalMap", 2);
        }
    };
}
