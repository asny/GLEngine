//
//  GLSkyBoxMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    class GLSkyboxMaterial : public GLMaterial
    {
        std::shared_ptr<GLTexture3D> texture;
    public:
        
        GLSkyboxMaterial(std::shared_ptr<GLTexture3D> _texture) : texture(_texture)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/skybox.vert",  "../GLEngine/shaders/skybox.frag");
        }
        
        bool should_draw(DrawPassMode draw_pass)
        {
            return draw_pass == FORWARD;
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            shader->depth_test(true);
            shader->cull_back_faces(true);
            
            auto texture_id = texture->use();
            GLUniform::use(shader, "texture0", texture_id);
            GLUniform::use(shader, "MVMatrix", view * model);
            GLUniform::use(shader, "PMatrix", projection);
        }
    };
}
