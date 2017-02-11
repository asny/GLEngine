//
//  Created by Asger Nyman Christiansen on 30/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLObject.h"
#include "GLRenderTarget.h"
#include "lights/GLLight.h"

namespace gle
{
    class GLNode
    {
    public:
        
        void add_leaf(std::shared_ptr<mesh::Mesh> geometry, std::shared_ptr<GLMaterial> material)
        {
            auto object = GLObject(geometry, material);
            objects.push_back(object);
        }
        
        void add_child(std::shared_ptr<GLNode> node)
        {
            children.push_back(node);
        }
        
        void remove_child(std::shared_ptr<GLNode> node)
        {
            
            auto pointer = std::find(children.begin(), children.end(), node);
            if(pointer == children.end())
                std::runtime_error("The node cannot be removed since it is not a child.");
            children.erase(pointer);
        }
        
    protected:
        virtual void draw(DrawPassMode draw_pass, const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            for (const GLObject& object : objects)
            {
                object.draw(draw_pass, camera_position, model, view, projection);
            }
            for (std::shared_ptr<const GLNode> child : children)
            {
                child->draw(draw_pass, camera_position, model, view, projection);
            }
        }
        
    private:
        std::vector<const GLObject> objects = std::vector<const GLObject>();
        std::vector<std::shared_ptr<GLNode>> children = std::vector<std::shared_ptr<GLNode>>();
    };
    
    class GLScene : public GLNode
    {
        std::vector<std::shared_ptr<GLLight>> lights = std::vector<std::shared_ptr<GLLight>>();
        GLRenderTarget shadow_render_target;
        
    public:
        
        GLScene() : shadow_render_target(GLRenderTarget(2400, 1400, 0, true))
        {
            
        }
        
        void add_light(std::shared_ptr<GLLight> light)
        {
            lights.push_back(light);
        }
        
        void draw(DrawPassMode draw_pass, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            auto model = glm::mat4(1.);
            GLNode::draw(draw_pass, camera_position, model, view, projection);
        }
        
        void shine_light(const glm::vec2& screen_size, const glm::vec3& camera_position,
                         const std::shared_ptr<GLTexture> position_texture, const std::shared_ptr<GLTexture> color_texture, const std::shared_ptr<GLTexture> normal_texture, const std::shared_ptr<GLTexture> depth_texture) const
        {
            for(auto light : lights)
            {
                // Bind buffer
                shadow_render_target.use();
                
                // Clear the buffer
                GLState::depth_write(true);
                glClear(GL_DEPTH_BUFFER_BIT);
                
                // Draw the scene
                glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
                glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(-5., 5., 0.), glm::vec3(0,0,0), glm::vec3(0,1,0));
                draw(SHADOW, camera_position, depthViewMatrix, depthProjectionMatrix);
                
                // Bind buffer
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
                
                // Set up blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
                
                // Draw the scene
                glm::mat4 biasMatrix(
                                     0.5, 0.0, 0.0, 0.0,
                                     0.0, 0.5, 0.0, 0.0,
                                     0.0, 0.0, 0.5, 0.0,
                                     0.5, 0.5, 0.5, 1.0
                                     );
                
                light->shine(screen_size, camera_position, biasMatrix * depthProjectionMatrix * depthViewMatrix,
                             position_texture, color_texture, normal_texture, depth_texture,
                             shadow_render_target.get_depth_texture());
            }
        }
    };
    
    class GLTransformationNode : public GLNode
    {
    public:
        GLTransformationNode(const glm::mat4& _transformation)
        {
            transformation = std::make_shared<glm::mat4>(_transformation);
        }
        
        GLTransformationNode(const std::shared_ptr<const glm::mat4> _transformation)
        {
            transformation = _transformation;
        }
        
    private:
        std::shared_ptr<const glm::mat4> transformation;
        
        void draw(DrawPassMode draw_pass, const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            GLNode::draw(draw_pass, camera_position, model * (*transformation), view, projection);
        }
    };
    
    class GLRotationNode : public GLNode
    {
    public:
        GLRotationNode(const glm::vec3& _axis, float _angle) : axis(_axis), angle(std::make_shared<float>(_angle))
        {
            
        }
        
        GLRotationNode(const glm::vec3& _axis, std::shared_ptr<float> _angle) : axis(_axis), angle(_angle)
        {
            
        }
        
    private:
        glm::vec3 axis;
        std::shared_ptr<float> angle;
        
        void draw(DrawPassMode draw_pass, const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            GLNode::draw(draw_pass, camera_position, model * glm::rotate(glm::mat4(1.f), *angle, axis), view, projection);
        }
    };
    
    class GLTranslationNode : public GLNode
    {
    public:
        GLTranslationNode(const glm::vec3& _translation) : translation(std::make_shared<glm::vec3>(_translation))
        {
            
        }
        
        GLTranslationNode(std::shared_ptr<glm::vec3> _translation) : translation(_translation)
        {
            
        }
        
    private:
        std::shared_ptr<glm::vec3> translation;
        
        void draw(DrawPassMode draw_pass, const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            GLNode::draw(draw_pass, camera_position, model * glm::translate(glm::mat4(1.f), *translation), view, projection);
        }
    };

}
