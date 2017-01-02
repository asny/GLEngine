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
        std::shared_ptr<GLUniform<glm::mat4>> MVPUniform;
        std::shared_ptr<GLUniform<glm::mat4>> MVUniform;
        std::shared_ptr<GLUniform<glm::mat4>> NUniform;
        
        std::shared_ptr<GLUniform<glm::vec3>> colorUniform;
        
    public:
        
        GLDeferredMaterial(const glm::vec3& color)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/geometry_pass.vert",  "../GLEngine/shaders/geometry_pass.frag");
            
            MVPUniform = shader->create_uniform("MVPMatrix", glm::mat4(1.));
            MVUniform = shader->create_uniform("MVMatrix", glm::mat4(1.));
            NUniform = shader->create_uniform("NMatrix", glm::mat4(1.));
            
            colorUniform = shader->create_uniform("materialColor", color);
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
            
            shader->use();
            MVUniform->use(modelView);
            MVPUniform->use(projection * modelView);
            NUniform->use(inverseTranspose(modelView));
            
            colorUniform->use();
        }
    };
}
