//
//  Created by Asger Nyman Christiansen on 29/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    class GLLight
    {
        GLuint array_id;
        
        std::shared_ptr<GLUniform<int>> positionMapUniform;
        std::shared_ptr<GLUniform<int>> colorMapUniform;
        std::shared_ptr<GLUniform<int>> normalMapUniform;
        std::shared_ptr<GLUniform<glm::vec2>> screenSizeUniform;
        
    protected:
        std::shared_ptr<GLShader> shader;
        
        GLLight(const std::string& fragment_shader_filename)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/light_pass.vert",  fragment_shader_filename);
            
            screenSizeUniform = shader->create_uniform("screenSize", glm::vec2(1,1));
            positionMapUniform = shader->create_uniform_int("positionMap", 0);
            colorMapUniform = shader->create_uniform_int("colorMap", 1);
            normalMapUniform = shader->create_uniform_int("normalMap", 2);
            
            // Generate and bind array
            glGenVertexArrays(1, &array_id);
            glBindVertexArray(array_id);
            
            // Create mesh
            auto mesh = std::make_shared<mesh::Mesh>();
            mesh::VertexID* v1 = mesh->create_vertex(glm::vec3(-3., -1., 0.));
            mesh::VertexID* v2 = mesh->create_vertex(glm::vec3(3., -1., 0.));
            mesh::VertexID* v3 = mesh->create_vertex(glm::vec3(0., 2., 0.));
            mesh->create_face(v1, v3, v2);
            
            // Create attribute and send data.
            auto attribute = shader->create_attribute("position", mesh->position());
            attribute->add_data_at(*v1);
            attribute->add_data_at(*v2);
            attribute->add_data_at(*v3);
            attribute->send_data();
        }
        
        virtual void use_light_properties()
        {
            
        }
        
    public:
        
        static void pre_shine()
        {
            // Do not write or test with the depth buffer
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);
            
            // Set up blending
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
        }
        
        void shine(const glm::vec2& screen_size)
        {
            shader->use();
            
            screenSizeUniform->use(screen_size);
            positionMapUniform->use();
            colorMapUniform->use();
            normalMapUniform->use();
            
            use_light_properties();
            
            // Bind vertex array and draw
            glBindVertexArray(array_id);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    };
    
    class GLDirectionalLight : public GLLight
    {
        std::shared_ptr<GLUniform<glm::vec3>> lightDirectionUniform;
    public:
        
        GLDirectionalLight(const glm::vec3& direction) :
            GLLight("../GLEngine/shaders/light_pass.frag")
        {
            
            lightDirectionUniform = shader->create_uniform("lightDirection", direction);
        }
        
        void use_light_properties()
        {
            lightDirectionUniform->use();
        }
    };
}
