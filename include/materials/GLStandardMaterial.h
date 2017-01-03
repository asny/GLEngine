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
        std::shared_ptr<GLUniform<glm::mat4>> MVPUniform;
        std::shared_ptr<GLUniform<glm::mat4>> MVUniform;
        std::shared_ptr<GLUniform<glm::mat4>> VUniform;
        std::shared_ptr<GLUniform<glm::mat4>> NUniform;
        
        std::shared_ptr<GLUniform<glm::vec3>> ambientUniform;
        std::shared_ptr<GLUniform<glm::vec3>> diffuseUniform;
        std::shared_ptr<GLUniform<glm::vec3>> specularUniform;
        std::shared_ptr<GLUniform<float>> opacityUniform;
        
    public:
        
        GLStandardMaterial(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, double _opacity)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/phong.vert",  "../GLEngine/shaders/phong.frag");
            
            VUniform = shader->create_uniform("VMatrix", glm::mat4(1.));
            MVUniform = shader->create_uniform("MVMatrix", glm::mat4(1.));
            NUniform = shader->create_uniform("NMatrix", glm::mat4(1.));
            MVPUniform = shader->create_uniform("MVPMatrix", glm::mat4(1.));
            ambientUniform = shader->create_uniform("ambientMat", _ambient);
            diffuseUniform = shader->create_uniform("diffuseMat", _diffuse);
            specularUniform = shader->create_uniform("specMat", _specular);
            opacityUniform = shader->create_uniform("opacity", _opacity);
            
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
            
            shader->use();
            VUniform->use(view);
            MVUniform->use(modelView);
            MVPUniform->use(projection * modelView);
            NUniform->use(inverseTranspose(modelView));
            
            ambientUniform->use();
            diffuseUniform->use();
            specularUniform->use();
            opacityUniform->use();
        }
    };
}
