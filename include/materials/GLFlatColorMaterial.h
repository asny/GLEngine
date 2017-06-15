//
//  GLFlatMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    
    class GLFlatColorMaterial : public GLMaterial
    {
        glm::vec3 color;
    public:
        
        GLFlatColorMaterial(const glm::vec3& _color) : GLMaterial(DEFERRED, "../GLEngine/shaders/pre_geom.vert",  "../GLEngine/shaders/color_material.frag", "../GLEngine/shaders/flat.geom"), color(_color)
        {
            
        }
        
        void pre_draw(const DrawPassInput& input, const glm::mat4& model)
        {
            auto shader = get_shader();
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(true);
            
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "VPMatrix", input.projection * input.view);
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
    
}
