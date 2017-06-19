//
//  GLTextureMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    class GLTextureMaterial : public GLMaterial
    {
        std::shared_ptr<GLTexture> texture;
        std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> uv_coordinates;
    public:
        bool cull_back_faces = true;
        
        GLTextureMaterial(std::shared_ptr<GLTexture> _texture, std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> _uv_coordinates) : GLMaterial(DEFERRED, "../GLEngine/shaders/texture.vert",  "../GLEngine/shaders/texture.frag"), texture(_texture), uv_coordinates(_uv_coordinates)
        {
            
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vertex_attributes)
        {
            auto shader = get_shader();
            GLMaterial::create_attributes(geometry, vertex_attributes);
            vertex_attributes.push_back(GLVertexAttribute<glm::vec2>::use(*shader, "uv_coordinates", uv_coordinates));
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vertex_attributes)
        {
            auto shader = get_shader();
            GLMaterial::create_attributes(geometry, vertex_attributes);
            vertex_attributes.push_back(GLVertexAttribute<glm::vec3>::use(*shader, "normal", geometry->normal()));
        }
        
        void pre_draw(const DrawPassInput& input, const glm::mat4& model)
        {
            auto shader = get_shader();
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(cull_back_faces);
            
            texture->use(0);
            GLUniform::use(shader, "texture0", 0);
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "MVPMatrix", input.projection * input.view * model);
            GLUniform::use(shader, "NMatrix", inverseTranspose(model));
        }
    };
}
