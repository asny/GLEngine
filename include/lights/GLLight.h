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
        
        void shine(const glm::vec2& screen_size, const glm::vec3& camera_position, const glm::mat4& depthMVP,
                   const std::shared_ptr<GLTexture> position_texture,
                   const std::shared_ptr<GLTexture> color_texture,
                   const std::shared_ptr<GLTexture> normal_texture,
                   const std::shared_ptr<GLTexture> depth_texture,
                   const std::shared_ptr<GLTexture> shadow_texture)
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
            shadow_texture->use(4);
            
            GLUniform::use(shader, "shadowMVP", depthMVP);
            GLUniform::use(shader, "eyePosition", camera_position);
            GLUniform::use(shader, "screenSize", screen_size);
            GLUniform::use(shader, "positionMap", 0);
            GLUniform::use(shader, "colorMap", 1);
            GLUniform::use(shader, "normalMap", 2);
            GLUniform::use(shader, "depthMap", 3);
            GLUniform::use(shader, "shadowMap", 4);
            
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
            GLLight(), direction(_direction)
        {
            
        }
        
        void use_light_properties()
        {
            GLUniform::use(shader, "lightType", 1);
            GLUniform::use(shader, "directionalLight.direction", direction);
            GLUniform::use(shader, "directionalLight.base.color", glm::vec3(1., 1., 1.));
            GLUniform::use(shader, "directionalLight.base.ambientIntensity", 0.2f);
            GLUniform::use(shader, "directionalLight.base.diffuseIntensity", 0.5f);
        }
    };
    
    class GLPointLight : public GLLight
    {
        glm::vec3 position;
    public:
        
        GLPointLight(const glm::vec3& _position) : GLLight(), position(_position)
        {
            
        }
        
        void use_light_properties()
        {
            GLUniform::use(shader, "lightType", 2);
            GLUniform::use(shader, "pointLight.position", position);
            GLUniform::use(shader, "pointLight.base.color", glm::vec3(1., 1., 1.));
            GLUniform::use(shader, "pointLight.base.ambientIntensity", 0.2f);
            GLUniform::use(shader, "pointLight.base.diffuseIntensity", 0.5f);
            GLUniform::use(shader, "pointLight.attenuation.constant", 1.f);
            GLUniform::use(shader, "pointLight.attenuation.linear", 0.1f);
            GLUniform::use(shader, "pointLight.attenuation.exp", 0.01f);
        }
    };
}
