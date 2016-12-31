//
//  Created by Asger Nyman Christiansen on 27/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLTexture.h"
#include "GLShader.h"
#include "GLUniform.h"
#include "GLVertexAttribute.h"

namespace gle
{
    class GLMaterial
    {
        std::vector<GLUniform<int>> int_uniforms;
        std::vector<GLUniform<float>> float_uniforms;
        std::vector<GLUniform<glm::vec2>> vec2_uniforms;
        std::vector<GLUniform<glm::vec3>> vec3_uniforms;
        std::vector<GLUniform<glm::vec4>> vec4_uniforms;
        std::vector<GLUniform<glm::mat4>> mat4_uniforms;
        
    protected:
        
        std::shared_ptr<glm::mat4> model = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> view = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> modelView = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> inverseModelView = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> projection = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> modelViewProjection = std::make_shared<glm::mat4>(1.);
        
        std::shared_ptr<glm::vec3> camera_position = std::make_shared<glm::vec3>(0.);
        
        bool cull_back_faces = true;
        bool test_depth = true;
        
        std::shared_ptr<GLShader> shader;
        
        void use_uniform_int(const std::string& name, const std::shared_ptr<int> value)
        {
            auto location = shader->get_uniform_location(name);
            if(location != NULL_LOCATION)
                int_uniforms.push_back(GLUniform<int>(location, value));
        }
        
        void use_uniform(const std::string& name, const std::shared_ptr<float> value)
        {
            auto location = shader->get_uniform_location(name);
            if(location != NULL_LOCATION)
                float_uniforms.push_back(GLUniform<float>(location, value));
        }
        
        void use_uniform(const std::string& name, const std::shared_ptr<glm::vec2> value)
        {
            auto location = shader->get_uniform_location(name);
            if(location != NULL_LOCATION)
                vec2_uniforms.push_back(GLUniform<glm::vec2>(location, value));
        }
        
        void use_uniform(const std::string& name, const std::shared_ptr<glm::vec3> value)
        {
            auto location = shader->get_uniform_location(name);
            if(location != NULL_LOCATION)
                vec3_uniforms.push_back(GLUniform<glm::vec3>(location, value));
        }
        
        void use_uniform(const std::string& name, const std::shared_ptr<glm::vec4> value)
        {
            auto location = shader->get_uniform_location(name);
            if(location != NULL_LOCATION)
                vec4_uniforms.push_back(GLUniform<glm::vec4>(location, value));
        }
        
        void use_uniform(const std::string& name, const std::shared_ptr<glm::mat4> value)
        {
            auto location = shader->get_uniform_location(name);
            if(location != NULL_LOCATION)
                mat4_uniforms.push_back(GLUniform<glm::mat4>(location, value));
        }
        
        void use_uniform_int(const std::string& name, const int& value)
        {
            use_uniform_int(name, std::make_shared<int>(value));
        }
        
        void use_uniform(const std::string& name, const float& value)
        {
            use_uniform(name, std::make_shared<float>(value));
        }
        
        void use_uniform(const std::string& name, const glm::vec2& value)
        {
            use_uniform(name, std::make_shared<glm::vec2>(value));
        }
        
        void use_uniform(const std::string& name, const glm::vec3& value)
        {
            use_uniform(name, std::make_shared<glm::vec3>(value));
        }
        
        void use_uniform(const std::string& name, const glm::vec4& value)
        {
            use_uniform(name, std::make_shared<glm::vec4>(value));
        }
        
        void use_uniform(const std::string& name, const glm::mat4& value)
        {
            use_uniform(name, std::make_shared<glm::mat4>(value));
        }
        
    public:
        
        virtual void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
        }
        
        virtual void pre_draw(const glm::vec3& _camera_position, const glm::mat4& _model, const glm::mat4& _view, const glm::mat4& _projection)
        {
            shader->use();
            
            // Cull back faces
            static bool currently_cull_back_faces = true;
            if(currently_cull_back_faces != cull_back_faces)
            {
                if(cull_back_faces)
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }
                else {
                    glDisable(GL_CULL_FACE);
                }
                currently_cull_back_faces = cull_back_faces;
            }
            
            // Depth test
            static bool currently_test_depth = true;
            if(currently_test_depth != test_depth)
            {
                if(test_depth)
                {
                    glDepthMask(GL_TRUE);
                }
                else {
                    glDepthMask(GL_FALSE);
                    
                }
                currently_test_depth = test_depth;
            }
            
            // Update standard uniforms related to object, camera and light.
            *model = _model;
            *view = _view;
            *modelView = _view * _model;
            *inverseModelView = inverseTranspose(*modelView);
            *projection = _projection;
            *modelViewProjection = _projection * (*modelView);
            *camera_position = _camera_position;
            
            // Use uniforms
            for (auto glUniform : int_uniforms)
            {
                glUniform.use();
            }
            
            for (auto glUniform : float_uniforms)
            {
                glUniform.use();
            }
            
            for (auto glUniform : vec2_uniforms)
            {
                glUniform.use();
            }
            
            for (auto glUniform : vec3_uniforms)
            {
                glUniform.use();
            }
            
            for (auto glUniform : vec4_uniforms)
            {
                glUniform.use();
            }
            
            for (auto glUniform : mat4_uniforms)
            {
                glUniform.use();
            }
        }
        
    };
}
