//
//  GLTextureMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace oogl
{
    class GLTextureMaterial : public GLMaterial
    {
        std::shared_ptr<GLTexture> texture;
        std::shared_ptr<geogo::Attribute<geogo::VertexID, glm::vec2>> uv_coordinates;
        std::shared_ptr<int> texture_id = std::make_shared<int>(0);
    public:
        
        GLTextureMaterial(std::shared_ptr<GLTexture> _texture, std::shared_ptr<geogo::Attribute<geogo::VertexID, glm::vec2>> _uv_coordinates) : texture(_texture), uv_coordinates(_uv_coordinates)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/texture.vert",  "../GLEngine/shaders/texture.frag");
            
            use_uniform_int("texture0", texture_id);
        }
        
        void create_attributes(std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec2_vertex_attributes.push_back(create_attribute("uv_coordinates", uv_coordinates));
        }
        
        void pre_draw()
        {
            *texture_id = texture->use();
            GLMaterial::pre_draw();
        }
    };
}
