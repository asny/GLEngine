//
//  Created by Asger Nyman Christiansen on 01/05/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once


#include "GLMaterial.h"

namespace gle
{
    
    class GLWireframeMaterial : public GLMaterial
    {
        glm::vec3 color;
        
    public:
        
        GLWireframeMaterial(const glm::vec3& _color) : GLMaterial(FORWARD), color(_color)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/pre_geom.vert",  "../GLEngine/shaders/wireframe.frag", "../GLEngine/shaders/wireframe.geom");
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(false);
            
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "VPMatrix", projection * view);
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
}
