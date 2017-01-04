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
    
    class GLColorMaterial : public GLMaterial
    {
        glm::vec3 color;
        
    public:
        
        GLColorMaterial(const glm::vec3& _color) : color(_color)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/color_material.vert",  "../GLEngine/shaders/color_material.frag");
        }
        
        bool should_draw(DrawPassMode draw_pass)
        {
            return draw_pass == DEFERRED;
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
            vec3_vertex_attributes.push_back(shader->create_attribute("normal", geometry->normal()));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            auto modelView = view * model;
            
            GLUniform<glm::mat4>::use(shader, "MVMatrix", modelView);
            GLUniform<glm::mat4>::use(shader, "MVPMatrix", projection * modelView);
            GLUniform<glm::mat4>::use(shader, "NMatrix", inverseTranspose(modelView));
            
            GLUniform<glm::vec3>::use(shader, "materialColor", color);
        }
    };
}
