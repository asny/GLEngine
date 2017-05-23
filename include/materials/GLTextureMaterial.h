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
        
        GLTextureMaterial(std::shared_ptr<GLTexture> _texture, std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> _uv_coordinates) : GLMaterial(DEFERRED), texture(_texture), uv_coordinates(_uv_coordinates)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/texture.vert",  "../GLEngine/shaders/texture.frag");
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vertex_attributes)
        {
            GLMaterial::create_attributes(geometry, vertex_attributes);
            vertex_attributes.push_back(shader->create_attribute("uv_coordinates", uv_coordinates));
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vertex_attributes)
        {
            GLMaterial::create_attributes(geometry, vertex_attributes);
            vertex_attributes.push_back(shader->create_attribute("normal", geometry->normal()));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(true);
            
            texture->use(0);
            GLUniform::use(shader, "texture0", 0);
            GLUniform::use(shader, "MMatrix", model);
            GLUniform::use(shader, "MVPMatrix", projection * view * model);
            GLUniform::use(shader, "NMatrix", inverseTranspose(model));
        }
    };
}
