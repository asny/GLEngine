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
            : GLMaterial(DEFERRED, "../GLEngine/shaders/color_material.vert",  "../GLEngine/shaders/color_material.frag"), normals(_normals), color(_color)
        {
            
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vertex_attributes)
        {
            auto shader = get_shader();
            GLMaterial::create_attributes(geometry, vertex_attributes);
            if(normals)
                vertex_attributes.push_back(GLVertexAttribute<glm::vec3>::use(*shader, "normal", normals));
            else
                vertex_attributes.push_back(GLVertexAttribute<glm::vec3>::use(*shader, "normal", geometry->normal()));
        }
        
        void pre_draw(const DrawPassInput& input, const glm::mat4& model)
        {
            auto shader = get_shader();
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(true);
            
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "MVPMatrix", input.projection * input.view * model);
            GLUniform::use(shader, "NMatrix", inverseTranspose(model));
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
}
