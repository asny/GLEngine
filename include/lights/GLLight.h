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
        glm::mat4 bias_matrix = glm::mat4(
                             0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0);
        
        std::shared_ptr<GLShader> get_shader()
        {
            return GLShader::create_or_get("../GLEngine/shaders/light_pass.vert",  "../GLEngine/shaders/light_pass.frag");
        }
        
        void shine(const DrawPassInput& input)
        {
            auto shader = get_shader();
            GLState::depth_write(false);
            GLState::depth_test(false);
            GLState::cull_back_faces(true);
            glDepthFunc(GL_LEQUAL);
            
            input.color_texture->use(0);
            GLUniform::use(shader, "colorMap", 0);
            input.position_texture->use(1);
            GLUniform::use(shader, "positionMap", 1);
            input.normal_texture->use(2);
            GLUniform::use(shader, "normalMap", 2);
            input.depth_texture->use(3);
            GLUniform::use(shader, "depthMap", 3);
            
            GLUniform::use(shader, "eyePosition", input.camera_position);
            
            GLObject::draw_full_screen_quad(shader);
        }
    public:
        glm::vec3 color = glm::vec3(1., 1., 1.);
        float ambient_intensity = 0.2f;
        float diffuse_intensity = 0.5f;
    };
    
    class GLDirectionalLight : public GLLight
    {
    public:
        glm::vec3 direction = glm::vec3(0., -1., 0.);
        float shadow_radius = 10.f;
        glm::vec3 shadow_target = glm::vec3(0., 0., 0.);
        
        glm::mat4 get_view()
        {
            glm::vec3 up = normalize(cross(direction, glm::vec3(1., 0., 0.)));
            return glm::lookAt(shadow_target - direction, shadow_target, up);
        }
        
        glm::mat4 get_projection()
        {
            return glm::ortho<float>(-shadow_radius, shadow_radius, -shadow_radius, shadow_radius, -2.f * shadow_radius, 2.f * shadow_radius);
        }
        
        void shine(const DrawPassInput& input, const GLShadowRenderTarget& shadow_render_target)
        {
            auto shader = get_shader();
            shadow_render_target.bind_texture_for_reading(4);
            GLUniform::use(shader, "shadowMap", 4);
            GLUniform::use(shader, "shadowCubeMap", 5);
            GLUniform::use(shader, "shadowMVP", bias_matrix * get_projection() * get_view());
            GLUniform::use(shader, "lightType", 1);
            GLUniform::use(shader, "directionalLight.direction", direction);
            GLUniform::use(shader, "directionalLight.base.color", color);
            GLUniform::use(shader, "directionalLight.base.ambientIntensity", ambient_intensity);
            GLUniform::use(shader, "directionalLight.base.diffuseIntensity", diffuse_intensity);
            
            GLLight::shine(input);
        }
    };
    
    class GLPointLight : public GLLight
    {
    public:
        glm::vec3 position = glm::vec3(0., 0., 0.);
        float attenuation_constant = 0.1f;
        float attenuation_linear = 0.01f;
        float attenuation_exp = 0.001f;
        
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
        
        void shine(const DrawPassInput& input, const GLShadowCubeRenderTarget& shadow_render_target)
        {
            auto shader = get_shader();
            GLUniform::use(shader, "shadowMap", 4);
            shadow_render_target.bind_texture_for_reading(5);
            GLUniform::use(shader, "shadowCubeMap", 5);
            GLUniform::use(shader, "shadowMVP0", bias_matrix * get_projection() * get_view(0));
            GLUniform::use(shader, "shadowMVP1", bias_matrix * get_projection() * get_view(1));
            GLUniform::use(shader, "shadowMVP2", bias_matrix * get_projection() * get_view(2));
            GLUniform::use(shader, "shadowMVP3", bias_matrix * get_projection() * get_view(3));
            GLUniform::use(shader, "shadowMVP4", bias_matrix * get_projection() * get_view(4));
            GLUniform::use(shader, "shadowMVP5", bias_matrix * get_projection() * get_view(5));
            GLUniform::use(shader, "lightType", 2);
            GLUniform::use(shader, "pointLight.position", position);
            GLUniform::use(shader, "pointLight.base.color", color);
            GLUniform::use(shader, "pointLight.base.ambientIntensity", ambient_intensity);
            GLUniform::use(shader, "pointLight.base.diffuseIntensity", diffuse_intensity);
            GLUniform::use(shader, "pointLight.attenuation.constant", attenuation_constant);
            GLUniform::use(shader, "pointLight.attenuation.linear", attenuation_linear);
            GLUniform::use(shader, "pointLight.attenuation.exp", attenuation_exp);
            
            GLLight::shine(input);
        }
    };
}
