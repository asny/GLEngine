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
        glm::mat4 bias_matrix = glm::mat4(
                             0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0);
        
        GLLight()
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/light_pass.vert",  "../GLEngine/shaders/light_pass.frag");
            
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
        
        void shine(const glm::vec3& view_position,
                   const std::shared_ptr<GLTexture> position_texture,
                   const std::shared_ptr<GLTexture> color_texture,
                   const std::shared_ptr<GLTexture> normal_texture,
                   const std::shared_ptr<GLTexture> depth_texture)
        {
            // Do not write or test with the depth buffer
            GLState::depth_write(true);
            GLState::depth_test(true);
            GLState::cull_back_faces(true);
            glDepthFunc(GL_LEQUAL);
            
            position_texture->use(0);
            color_texture->use(1);
            normal_texture->use(2);
            depth_texture->use(3);
            
            GLUniform::use(shader, "eyePosition", view_position);
            GLUniform::use(shader, "positionMap", 0);
            GLUniform::use(shader, "colorMap", 1);
            GLUniform::use(shader, "normalMap", 2);
            GLUniform::use(shader, "depthMap", 3);
            
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
        
        GLDirectionalLight(const glm::vec3& _direction) : GLLight(), direction(_direction)
        {
            
        }
        
        glm::mat4 get_view(const glm::vec3& target)
        {
            const float distance = 1.f;
            glm::vec3 up = normalize(cross(direction, glm::vec3(1., 0., 0.)));
            return glm::lookAt(target - distance * direction, target, up);
        }
        
        glm::mat4 get_projection()
        {
            return glm::ortho<float>(-10,10,-10,10,-10,20);
        }
        
        void shine(const glm::vec3& view_position,
                   const glm::vec3& target,
                   const std::shared_ptr<GLTexture> position_texture,
                   const std::shared_ptr<GLTexture> color_texture,
                   const std::shared_ptr<GLTexture> normal_texture,
                   const std::shared_ptr<GLTexture> depth_texture,
                   const GLRenderTarget& shadow_render_target)
        {
            shadow_render_target.get_depth_texture()->use(4);
            GLUniform::use(shader, "shadowMap", 4);
            GLUniform::use(shader, "shadowMVP", bias_matrix * get_projection() * get_view(target));
            GLUniform::use(shader, "lightType", 1);
            GLUniform::use(shader, "directionalLight.direction", direction);
            GLUniform::use(shader, "directionalLight.base.color", glm::vec3(1., 1., 1.));
            GLUniform::use(shader, "directionalLight.base.ambientIntensity", 0.2f);
            GLUniform::use(shader, "directionalLight.base.diffuseIntensity", 0.5f);
            
            GLLight::shine(view_position, position_texture, color_texture, normal_texture, depth_texture);
        }
    };
    
    class GLPointLight : public GLLight
    {
        glm::vec3 position;
    public:
        
        GLPointLight(const glm::vec3& _position) : GLLight(), position(_position)
        {
            
        }
        
        glm::mat4 get_view(int layer)
        {
            switch (layer) {
                case 0:
                    return glm::lookAt(position, position + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0));
                case 1:
                    return glm::lookAt(position, position + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0));
                case 2:
                    return glm::lookAt(position, position + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0));
                case 3:
                    return glm::lookAt(position, position + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0));
                case 4:
                    return glm::lookAt(position, position + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0));
                case 5:
                    return glm::lookAt(position, position + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0));
            }
            return glm::mat4(1.);
        }
        
        glm::mat4 get_projection()
        {
            return glm::perspective<float>(45.0f, 1.0f, 1.0f, 50.0f);
        }
        
        void shine(const glm::vec3& view_position,
                   const std::shared_ptr<GLTexture> position_texture,
                   const std::shared_ptr<GLTexture> color_texture,
                   const std::shared_ptr<GLTexture> normal_texture,
                   const std::shared_ptr<GLTexture> depth_texture,
                   const GLRenderTarget& shadow_render_target)
        {
            shadow_render_target.get_depth_3d_texture()->use(5);
            GLUniform::use(shader, "shadowCubeMap", 5);
            GLUniform::use(shader, "shadowMVP", bias_matrix * get_projection() * get_view(0));
            GLUniform::use(shader, "lightType", 2);
            GLUniform::use(shader, "pointLight.position", position);
            GLUniform::use(shader, "pointLight.base.color", glm::vec3(1., 1., 1.));
            GLUniform::use(shader, "pointLight.base.ambientIntensity", 0.2f);
            GLUniform::use(shader, "pointLight.base.diffuseIntensity", 0.5f);
            GLUniform::use(shader, "pointLight.attenuation.constant", 0.1f);
            GLUniform::use(shader, "pointLight.attenuation.linear", 0.01f);
            GLUniform::use(shader, "pointLight.attenuation.exp", 0.001f);
            
            GLLight::shine(view_position, position_texture, color_texture, normal_texture, depth_texture);
        }
    };
}
