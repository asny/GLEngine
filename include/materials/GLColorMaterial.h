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
        const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> normals;
        
    public:
        
        GLColorMaterial(const glm::vec3& _color, const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> _normals = nullptr)
            : normals(_normals), color(_color)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/color_material.vert",  "../GLEngine/shaders/color_material.frag");
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
            if(normals)
                vec3_vertex_attributes.push_back(shader->create_attribute("normal", normals));
            else
                vec3_vertex_attributes.push_back(shader->create_attribute("normal", geometry->normal()));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(true);
            
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "MVPMatrix", projection * view * model);
            GLUniform::use(shader, "NMatrix", inverseTranspose(model));
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
}
