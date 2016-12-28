//
//  GLDeferredMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 27/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    
    class GLDeferredMaterial : public GLMaterial
    {
    public:
        
        GLDeferredMaterial()
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/geometry_pass.vert",  "../GLEngine/shaders/geometry_pass.frag");
            
            use_uniform("MVPMatrix", modelViewProjection);
            use_uniform("MMatrix", model);
        }
    };
    
}
