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
        
        std::shared_ptr<GLNode> add_child(std::shared_ptr<GLNode> node)
        {
            children.push_back(node);
            return node;
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
        std::vector<std::shared_ptr<GLDirectionalLight>> directional_lights;
        std::vector<std::shared_ptr<GLPointLight>> point_lights;
        GLRenderTarget point_light_shadow_render_target;
        GLRenderTarget directional_light_shadow_render_target;
        
    public:
        
        GLScene() : directional_light_shadow_render_target(GLRenderTarget(1024, 1024, 0, true))
        {
            point_light_shadow_render_target.create_depth_cubemap(1024, 1024);
        }
        
        void add_light(std::shared_ptr<GLPointLight> light)
        {
            point_lights.push_back(light);
        }
        
        void add_light(std::shared_ptr<GLDirectionalLight> light)
        {
            directional_lights.push_back(light);
        }
        
        void draw(DrawPassMode draw_pass, const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            auto model = glm::mat4(1.);
            GLNode::draw(draw_pass, camera_position, model, view, projection);
        }
        
        void shine_light(const glm::vec3& view_position, const glm::vec3& view_direction,
                         const GLRenderTarget& source_render_target, const GLDefaultRenderTarget& render_target) const
        {
            // Set up blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            
            for(auto light : point_lights)
            {
                // Cast shadows
                point_light_shadow_render_target.use();
                for (int i = 0; i < 6; i++)
                {
                    point_light_shadow_render_target.bind_depth_texture_cubemap_for_writing(i);
                    point_light_shadow_render_target.clear();
                    draw(DEFERRED, view_position, light->get_view(i), light->get_projection());
                }
                
                // Shine the light
                render_target.use();
                light->shine(view_position, source_render_target, point_light_shadow_render_target);
            }
            
            for(auto light : directional_lights)
            {
                // Cast shadows
                directional_light_shadow_render_target.use();
                directional_light_shadow_render_target.clear();
                draw(DEFERRED, view_position, light->get_view(view_position, view_direction), light->get_projection());
                
                // Shine the light
                render_target.use();
                light->shine(view_position, view_direction, source_render_target, directional_light_shadow_render_target);
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
