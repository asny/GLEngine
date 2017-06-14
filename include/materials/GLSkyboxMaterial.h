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
        
        GLSkyboxMaterial(std::shared_ptr<GLTexture3D> _texture) : GLMaterial(DEFERRED, "../GLEngine/shaders/skybox.vert",  "../GLEngine/shaders/skybox.frag"), texture(_texture)
        {
            
        }
        
        void pre_draw(const DrawPassInput& input, const glm::mat4& model)
        {
            auto shader = get_shader();
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(true);
            glDepthFunc(GL_LEQUAL);
            
            texture->use(0);
            GLUniform::use(shader, "texture0", 0);
            GLUniform::use(shader, "eyePosition", input.camera_position);
            GLUniform::use(shader, "MVMatrix", input.view * model);
            GLUniform::use(shader, "PMatrix", input.projection);
        }
    };
}
