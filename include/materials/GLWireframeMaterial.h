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
        
        GLWireframeMaterial(const glm::vec3& _color) : GLMaterial(FORWARD, "../GLEngine/shaders/pre_geom.vert",  "../GLEngine/shaders/wireframe.frag", "../GLEngine/shaders/wireframe.geom"), color(_color)
        {
            
        }
        
        void pre_draw(const DrawPassInput& input, const glm::mat4& model)
        {
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(false);
            
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "VPMatrix", input.projection * input.view);
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
}
