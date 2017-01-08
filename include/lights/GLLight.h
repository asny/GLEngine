//
//  Created by Asger Nyman Christiansen on 29/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "Mesh.h"
#include "GLShader.h"
#include "GLUniform.h"

namespace gle
{
    class GLLight
    {
        GLuint array_id;
        
    protected:
        std::shared_ptr<GLShader> shader;
        
        GLLight(const std::string& fragment_shader_filename)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/light_pass.vert",  fragment_shader_filename);
            
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
        
        void shine(const glm::vec2& screen_size)
        {
            GLUniform::use(shader, "screenSize", screen_size);
            GLUniform::use(shader, "positionMap", 0);
            GLUniform::use(shader, "colorMap", 1);
            GLUniform::use(shader, "normalMap", 2);
            
            use_light_properties();
            
            // Bind vertex array and draw
            shader->use();
            glBindVertexArray(array_id);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    };
    
    class GLDirectionalLight : public GLLight
    {
        glm::vec3 direction;
    public:
        
        GLDirectionalLight(const glm::vec3& _direction) :
            GLLight("../GLEngine/shaders/light_pass.frag"), direction(_direction)
        {
            
        }
        
        void use_light_properties()
        {
            GLUniform::use(shader, "lightDirection", direction);
        }
    };
}
