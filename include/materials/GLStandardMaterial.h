//
//  GLStandardMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    class GLStandardMaterial : public GLMaterial
    {
        glm::vec3 ambient, diffuse, specular;
        float opacity;
        const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> normals;
        
    public:
        
        GLStandardMaterial(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, double _opacity,
                           const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> _normals = nullptr)
            : GLMaterial(FORWARD, "../GLEngine/shaders/phong.vert",  "../GLEngine/shaders/phong.frag"), normals(_normals), ambient(_ambient), diffuse(_diffuse), specular(_specular), opacity(_opacity)
        {
            
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vertex_attributes)
        {
            auto shader = get_shader();
            GLMaterial::create_attributes(geometry, vertex_attributes);
            if(normals)
                vertex_attributes.push_back(shader->create_attribute("normal", normals));
            else
                vertex_attributes.push_back(shader->create_attribute("normal", geometry->normal()));
        }
        
        void pre_draw(const DrawPassInput& input, const glm::mat4& model)
        {
            auto shader = get_shader();
            GLState::depth_test(true);
            GLState::depth_write(opacity >= 0.999);
            GLState::cull_back_faces(true);
            
            auto modelView = input.view * model;
            
            GLUniform::use(shader, "VMatrix", input.view);
            GLUniform::use(shader, "MVMatrix", modelView);
            GLUniform::use(shader, "MVPMatrix", input.projection * modelView);
            GLUniform::use(shader, "NMatrix", inverseTranspose(modelView));
            
            GLUniform::use(shader, "ambientMat", ambient);
            GLUniform::use(shader, "diffuseMat", diffuse);
            GLUniform::use(shader, "specMat", specular);
            
            GLUniform::use(shader, "opacity", opacity);
        }
    };
}
