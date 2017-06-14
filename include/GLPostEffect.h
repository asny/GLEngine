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
        std::string vertex_shader_name;
        std::string fragment_shader_name;
    protected:
        std::shared_ptr<GLShader> get_shader() const
        {
            return GLShader::create_or_get(vertex_shader_name, fragment_shader_name);
        }
        
    public:
        GLPostEffect(const std::string& _vertex_shader_name, const std::string& _fragment_shader_name) : vertex_shader_name(_vertex_shader_name),fragment_shader_name(_fragment_shader_name)
        {
            
        }
        
        virtual void apply(const DrawPassInput& input) const = 0;
    };
}
