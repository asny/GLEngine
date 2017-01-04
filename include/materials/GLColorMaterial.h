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
        
        GLColorMaterial(const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> _normals,
                        const glm::vec3& _color) : normals(_normals), color(_color)
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
            vec3_vertex_attributes.push_back(shader->create_attribute("normal", normals));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            auto modelView = view * model;
            
            GLUniform::use(shader, "MVMatrix", modelView);
            GLUniform::use(shader, "MVPMatrix", projection * modelView);
            GLUniform::use(shader, "NMatrix", inverseTranspose(modelView));
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
}
