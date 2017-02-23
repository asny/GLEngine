//
//  Created by Asger Nyman Christiansen on 22/02/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "Mesh.h"
#include "GLShader.h"
#include "GLUniform.h"

namespace gle
{
    class GLPostEffect
    {
    protected:
        std::shared_ptr<GLShader> shader;
        
    public:
        GLPostEffect(const std::string& vertex_shader, const std::string& fragment_shader)
        {
            shader = GLShader::create_or_get(vertex_shader, fragment_shader);
        }
        
        virtual void apply(const GLRenderTarget& source_render_target, float z_near, float z_far)
        {
            source_render_target.bind_color_texture_for_reading(0, 0);
            
            GLUniform::use(shader, "colorMap", 0);
            
            // Bind vertex array and draw
            shader->use();
            draw_full_screen_quad();
        }
    private:
        
        void draw_full_screen_quad()
        {
            static GLuint array_id = NULL_LOCATION;
            if(array_id == NULL_LOCATION)
            {
                // Generate and bind array
                glGenVertexArrays(1, &array_id);
                glBindVertexArray(array_id);
                
                // Create mesh
                auto mesh = std::make_shared<mesh::Mesh>();
                mesh::VertexID* v1 = mesh->create_vertex(glm::vec3(-3., -1., 0.));
                mesh::VertexID* v2 = mesh->create_vertex(glm::vec3(3., -1., 0.));
                mesh::VertexID* v3 = mesh->create_vertex(glm::vec3(0., 2., 0.));
                mesh->create_face(v1, v3, v2);
                
                auto uv_coordinates = std::make_shared<mesh::Attribute<mesh::VertexID, glm::vec2>>();
                uv_coordinates->at(v1) = glm::vec2(-1., 0.);
                uv_coordinates->at(v2) = glm::vec2(2., 0.);
                uv_coordinates->at(v3) = glm::vec2(0.5, 1.5);
                
                // Create attribute and send data.
                auto position = shader->create_attribute("position", mesh->position());
                position->add_data_at(*v1);
                position->add_data_at(*v2);
                position->add_data_at(*v3);
                position->send_data();
                
                auto uv = shader->create_attribute("uv_coordinates", uv_coordinates);
                uv->add_data_at(*v1);
                uv->add_data_at(*v2);
                uv->add_data_at(*v3);
                uv->send_data();
            }
            glBindVertexArray(array_id);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    };
    
    class GLFogEffect : public GLPostEffect
    {
        std::shared_ptr<GLTexture> noise_texture;
        std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> uv_coordinates;
        std::shared_ptr<float> time;
    public:
        GLFogEffect(std::shared_ptr<GLTexture> _noise_texture, std::shared_ptr<float> _time)
            : GLPostEffect("../GLEngine/shaders/fog_effect.vert",  "../GLEngine/shaders/fog_effect.frag"), time(_time), noise_texture(_noise_texture)
        {
            
        }
        
        void apply(const GLRenderTarget& source_render_target, float z_near, float z_far)
        {
            source_render_target.bind_depth_texture_for_reading(1);
            noise_texture->use(2);
            
            GLUniform::use(shader, "depthMap", 1);
            GLUniform::use(shader, "noiseTexture", 2);
            GLUniform::use(shader, "time", *time);
            GLUniform::use(shader, "zNear", z_near);
            GLUniform::use(shader, "zFar", z_far);
            
            GLPostEffect::apply(source_render_target, z_near, z_far);
        }
    };
}
