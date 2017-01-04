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
        
        GLTextureMaterial(std::shared_ptr<GLTexture> _texture, std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> _uv_coordinates) : texture(_texture), uv_coordinates(_uv_coordinates)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/texture.vert",  "../GLEngine/shaders/texture.frag");
        }
        
        bool should_draw(DrawPassMode draw_pass)
        {
            return draw_pass == FORWARD;
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
            vec2_vertex_attributes.push_back(shader->create_attribute("uv_coordinates", uv_coordinates));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            gle::GLMaterial::pre_draw();
            auto texture_id = texture->use();
            GLUniform::use(shader, "texture0", texture_id);
            GLUniform::use(shader, "MVPMatrix", projection * view * model);
        }
    };
}
