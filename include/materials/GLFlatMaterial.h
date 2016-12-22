//
//  GLFlatMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace oogl
{
    
    class GLFlatMaterial : public GLMaterial
    {
    public:
        
        GLFlatMaterial(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, double _opacity)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/pre_geom.vert",  "../GLEngine/shaders/phong.frag", "../GLEngine/shaders/flat.geom");
            
            use_uniform("VMatrix", view);
            use_uniform("MVMatrix", modelView);
            use_uniform("PMatrix", projection);
            use_uniform("ambientMat", _ambient);
            use_uniform("diffuseMat", _diffuse);
            use_uniform("specMat", _specular);
            use_uniform("opacity", _opacity);
            
            test_depth = _opacity >= 0.999;
        }
    };
    
}
