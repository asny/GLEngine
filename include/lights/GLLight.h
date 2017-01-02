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
        
        static std::shared_ptr<mesh::Mesh> create_screen_quad()
        {
            auto mesh = std::make_shared<mesh::Mesh>();
            
            mesh::VertexID* v1 = mesh->create_vertex(glm::vec3(-1., -1., 0.));
            mesh::VertexID* v2 = mesh->create_vertex(glm::vec3(-1., 1., 0.));
            mesh::VertexID* v3 = mesh->create_vertex(glm::vec3(1., 1., 0.));
            mesh::VertexID* v4 = mesh->create_vertex(glm::vec3(1., -1., 0.));
            mesh->create_face(v1, v3, v2);
            mesh->create_face(v3, v1, v4);
            
            return mesh;
        }
        
    protected:
        std::shared_ptr<GLShader> shader;
        
        GLLight(const std::string& vertex_shader_filename, const std::string& fragment_shader_filename)
        {
            shader = GLShader::create_or_get(vertex_shader_filename, fragment_shader_filename);
            
            screenSizeUniform = shader->create_uniform("screenSize", glm::vec2(2400, 1400));
            positionMapUniform = shader->create_uniform_int("positionMap", 0);
            colorMapUniform = shader->create_uniform_int("colorMap", 1);
            normalMapUniform = shader->create_uniform_int("normalMap", 2);
            
            // Generate and bind array
            glGenVertexArrays(1, &array_id);
            glBindVertexArray(array_id);
            
            auto mesh = create_screen_quad();
            auto attribute = shader->create_attribute("position", mesh->position());
            
            for(auto face = mesh->faces_begin(); face != mesh->faces_end(); face = face->next())
            {
                attribute->add_data_at(*face->v1());
                attribute->add_data_at(*face->v2());
                attribute->add_data_at(*face->v3());
            }
            
            attribute->send_data();
        }
        
        virtual void use_light_properties()
        {
            
        }
        
    public:
        
        void shine()
        {
            shader->use();
            
            screenSizeUniform->use();
            positionMapUniform->use();
            colorMapUniform->use();
            normalMapUniform->use();
            
            use_light_properties();
            
            // Bind vertex array and draw
            glBindVertexArray(array_id);
            glDrawArrays(GL_TRIANGLES, 0, 6);
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
        
        void use_light_properties()
        {
            lightDirectionUniform->use();
        }
    };
}
