//
//  Created by Asger Nyman Christiansen on 22/02/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "Mesh.h"
#include "GLShader.h"
#include "GLUniform.h"

namespace gle
{
    class GLPostEffect
    {
    protected:
        std::shared_ptr<GLShader> shader;
        
    public:
        GLPostEffect(const std::string& vertex_shader, const std::string& fragment_shader)
        {
            shader = GLShader::create_or_get(vertex_shader, fragment_shader);
        }
        
        virtual void apply(const GLRenderTarget& source_render_target, float z_near, float z_far)
        {
            GLState::depth_write(false);
            GLState::depth_test(false);
            
            source_render_target.bind_color_texture_for_reading(1, 0);
            source_render_target.bind_color_texture_for_reading(2, 1);
            source_render_target.bind_depth_texture_for_reading(2);
            
            GLUniform::use(shader, "positionMap", 0);
            GLUniform::use(shader, "normalMap", 1);
            GLUniform::use(shader, "depthMap", 2);
            
            GLObject::draw_full_screen_quad(shader);
        }
    };
}
