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
        }
        
        void shine(const glm::vec3& view_position, const GLRenderTarget& source_render_target)
        {
            GLState::depth_write(true);
            GLState::depth_test(true);
            GLState::cull_back_faces(true);
            glDepthFunc(GL_LEQUAL);
            
            source_render_target.bind_color_texture_for_reading(0, 0);
            source_render_target.bind_color_texture_for_reading(1, 1);
            source_render_target.bind_color_texture_for_reading(2, 2);
            source_render_target.bind_depth_texture_for_reading(3);
            
            GLUniform::use(shader, "eyePosition", view_position);
            GLUniform::use(shader, "colorMap", 0);
            GLUniform::use(shader, "positionMap", 1);
            GLUniform::use(shader, "normalMap", 2);
            GLUniform::use(shader, "depthMap", 3);
            
            GLObject::draw_full_screen_quad(shader);
        }
    };
    
    class GLDirectionalLight : public GLLight
    {
        glm::vec3 direction;
    public:
        
        GLDirectionalLight(const glm::vec3& _direction) : GLLight(), direction(_direction)
        {
            
        }
        
        glm::mat4 get_view(const glm::vec3& view_position, const glm::vec3& view_direction)
        {
            auto target = view_position + view_direction * 5.f;
            const float distance = 1.f;
            glm::vec3 up = normalize(cross(direction, glm::vec3(1., 0., 0.)));
            return glm::lookAt(target - distance * direction, target, up);
        }
        
        glm::mat4 get_projection()
        {
            return glm::ortho<float>(-10,10,-10,10,-10,20);
        }
        
        void shine(const glm::vec3& view_position, const glm::vec3& view_direction, const GLRenderTarget& source_render_target, const GLRenderTarget& shadow_render_target)
        {
            shadow_render_target.bind_depth_texture_for_reading(4);
            GLUniform::use(shader, "shadowMap", 4);
            GLUniform::use(shader, "shadowMVP", bias_matrix * get_projection() * get_view(view_position, view_direction));
            GLUniform::use(shader, "lightType", 1);
            GLUniform::use(shader, "directionalLight.direction", direction);
            GLUniform::use(shader, "directionalLight.base.color", glm::vec3(1., 1., 1.));
            GLUniform::use(shader, "directionalLight.base.ambientIntensity", 0.2f);
            GLUniform::use(shader, "directionalLight.base.diffuseIntensity", 0.5f);
            
            GLLight::shine(view_position, source_render_target);
        }
    };
    
    class GLPointLight : public GLLight
    {
    public:
        glm::vec3 position = glm::vec3(0., 0., 0.);
        
        GLPointLight() : GLLight()
        {
            
        }
        
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
            return glm::perspective<float>(glm::radians(90.0f), 1.0f, 1.0f, 50.0f);
        }
        
        void shine(const glm::vec3& view_position, const GLRenderTarget& source_render_target, const GLRenderTarget& shadow_render_target)
        {
            shadow_render_target.bind_depth_texture_cubemap_for_reading(4);
            GLUniform::use(shader, "shadowCubeMap", 4);
            GLUniform::use(shader, "shadowMVP0", bias_matrix * get_projection() * get_view(0));
            GLUniform::use(shader, "shadowMVP1", bias_matrix * get_projection() * get_view(1));
            GLUniform::use(shader, "shadowMVP2", bias_matrix * get_projection() * get_view(2));
            GLUniform::use(shader, "shadowMVP3", bias_matrix * get_projection() * get_view(3));
            GLUniform::use(shader, "shadowMVP4", bias_matrix * get_projection() * get_view(4));
            GLUniform::use(shader, "shadowMVP5", bias_matrix * get_projection() * get_view(5));
            GLUniform::use(shader, "lightType", 2);
            GLUniform::use(shader, "pointLight.position", position);
            GLUniform::use(shader, "pointLight.base.color", glm::vec3(1., 1., 1.));
            GLUniform::use(shader, "pointLight.base.ambientIntensity", 0.2f);
            GLUniform::use(shader, "pointLight.base.diffuseIntensity", 0.5f);
            GLUniform::use(shader, "pointLight.attenuation.constant", 0.1f);
            GLUniform::use(shader, "pointLight.attenuation.linear", 0.01f);
            GLUniform::use(shader, "pointLight.attenuation.exp", 0.001f);
            
            GLLight::shine(view_position, source_render_target);
        }
    };
}
