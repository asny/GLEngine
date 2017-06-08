//
//  Created by Asger Nyman Christiansen on 22/02/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "Mesh.h"
#include "GLShader.h"
#include "GLUniform.h"
#include "GLRenderTarget.h"
#include "GLObject.h"

namespace gle
{
    class GLPostEffect
    {
    public:
        GLPostEffect(const std::string& vertex_shader, const std::string& fragment_shader)
        {
            shader = GLShader::create_or_get(vertex_shader, fragment_shader);
        }
        
        virtual void apply(const DrawPassInput& input)  const = 0;
        
    protected:
        std::shared_ptr<GLShader> shader;
    };
}
