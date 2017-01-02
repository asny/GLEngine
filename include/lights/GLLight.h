//
//  Created by Asger Nyman Christiansen on 29/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    class GLLight : public GLMaterial
    {
        std::shared_ptr<GLUniform<int>> positionMapUniform;
        std::shared_ptr<GLUniform<int>> colorMapUniform;
        std::shared_ptr<GLUniform<int>> normalMapUniform;
        std::shared_ptr<GLUniform<glm::vec2>> screenSizeUniform;
        
    protected:
        std::shared_ptr<GLShader> shader;
        
        GLLight(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename)
        {
            shader = GLShader::create_or_get(vertex_shader_filename, fragment_shader_filename);
            
            screenSizeUniform = shader->create_uniform("screenSize", glm::vec2(2400, 1400));
            positionMapUniform = shader->create_uniform_int("positionMap", 0);
            colorMapUniform = shader->create_uniform_int("colorMap", 1);
            normalMapUniform = shader->create_uniform_int("normalMap", 2);
        }
        
    public:
        
        virtual void pre_draw()
        {
            shader->use();
            
            screenSizeUniform->use();
            positionMapUniform->use();
            colorMapUniform->use();
            normalMapUniform->use();
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                                       std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
        }
    };
    
    class GLDirectionalLight : public GLLight
    {
        std::shared_ptr<GLUniform<glm::vec3>> lightDirectionUniform;
    public:
        
        GLDirectionalLight(const glm::vec3& direction) :
            GLLight("../GLEngine/shaders/light_pass.vert",  "../GLEngine/shaders/light_pass.frag")
        {
            
            lightDirectionUniform = shader->create_uniform("lightDirection", direction);
        }
        
        void pre_draw()
        {
            GLLight::pre_draw();
            lightDirectionUniform->use();
        }
    };
}
