//
//  Created by Asger Nyman Christiansen on 27/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLTexture.h"
#include "GLUniform.h"
#include "GLState.h"
#include "GLRenderTarget.h"

namespace gle
{
    enum DrawPassMode {FORWARD, DEFERRED};
    
    struct DrawPassInput
    {
        const glm::vec3& camera_position;
        const glm::vec2& screen_size;
        const glm::mat4& view;
        const glm::mat4& projection;
        
        const GLTexture *color_texture = nullptr;
        const GLTexture *shaded_color_texture = nullptr;
        const GLTexture *position_texture = nullptr;
        const GLTexture *normal_texture = nullptr;
        const GLTexture *depth_texture = nullptr;
        
        DrawPassInput(const glm::vec3& _camera_position, const glm::vec2& _screen_size, const glm::mat4& _view, const glm::mat4& _projection, const GLColorRenderTarget *_geometry_pass_render_target = nullptr, const GLColorRenderTarget *_light_pass_render_target = nullptr)
            : camera_position(_camera_position), screen_size(_screen_size), view(_view), projection(_projection)
        {
            
        }
    };
    
    class GLMaterial
    {
        DrawPassMode mode = DEFERRED;
    protected:
        const std::shared_ptr<GLShader> shader;
        
        GLMaterial(DrawPassMode _mode, const std::string& vertex_shader_name, const std::string& fragment_shader_name, const std::string& geometry_shader_name = "") : mode(_mode), shader(GLShader::create_or_get(vertex_shader_name, fragment_shader_name, geometry_shader_name))
        {
            
        }
        
    public:
        
        bool should_draw(DrawPassMode draw_pass)
        {
            return mode == draw_pass;
        }
        
        virtual void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<float>>>& vertex_attributes)
        {
            
        }
        
        virtual void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vertex_attributes)
        {
            
        }
        
        virtual void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vertex_attributes)
        {
            vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
        }
        
        virtual void pre_draw(const DrawPassInput& input, const glm::mat4& model) = 0;
    };
}
