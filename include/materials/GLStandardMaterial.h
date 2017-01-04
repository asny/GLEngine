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
        
    public:
        
        GLStandardMaterial(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, double _opacity)
            : ambient(_ambient), diffuse(_diffuse), specular(_specular), opacity(_opacity)
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
            vec3_vertex_attributes.push_back(shader->create_attribute("normal", geometry->normal()));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            auto modelView = view * model;
            
            GLUniform<glm::mat4>::use(shader, "VMatrix", view);
            GLUniform<glm::mat4>::use(shader, "MVMatrix", modelView);
            GLUniform<glm::mat4>::use(shader, "MVPMatrix", projection * modelView);
            GLUniform<glm::mat4>::use(shader, "NMatrix", inverseTranspose(modelView));
            
            GLUniform<glm::vec3>::use(shader, "ambientMat", ambient);
            GLUniform<glm::vec3>::use(shader, "diffuseMat", diffuse);
            GLUniform<glm::vec3>::use(shader, "specMat", specular);
            
            GLUniform<float>::use(shader, "opacity", opacity);
        }
    };
}
