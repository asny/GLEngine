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
        
        GLStandardMaterial(const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> _normals,
                           const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, double _opacity)
            : normals(_normals), ambient(_ambient), diffuse(_diffuse), specular(_specular), opacity(_opacity)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/phong.vert",  "../GLEngine/shaders/phong.frag");
            test_depth = _opacity >= 0.999;
        }
        
        bool should_draw(DrawPassMode draw_pass)
        {
            return draw_pass == FORWARD;
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
            
            GLUniform::use(shader, "VMatrix", view);
            GLUniform::use(shader, "MVMatrix", modelView);
            GLUniform::use(shader, "MVPMatrix", projection * modelView);
            GLUniform::use(shader, "NMatrix", inverseTranspose(modelView));
            
            GLUniform::use(shader, "ambientMat", ambient);
            GLUniform::use(shader, "diffuseMat", diffuse);
            GLUniform::use(shader, "specMat", specular);
            
            GLUniform::use(shader, "opacity", opacity);
        }
    };
}
