//
//  GLSkyBoxMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace oogl
{
    class GLSkyboxMaterial : public GLMaterial
    {
        std::shared_ptr<GLTexture3D> texture;
        std::shared_ptr<int> texture_id = std::make_shared<int>(0);
    public:
        
        GLSkyboxMaterial(std::shared_ptr<GLTexture3D> _texture) : texture(_texture)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/skybox.vert",  "../GLEngine/shaders/skybox.frag");
            
            use_uniform_int("texture0", texture_id);
        }
        
        void pre_draw()
        {
            *texture_id = texture->use();
            GLMaterial::pre_draw();
        }
    };
}
