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
    protected:
        GLNode()
        {
            
        }
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
        virtual void draw(DrawPassInput& input, const glm::mat4& model) const
        {
            for (const GLObject& object : objects)
            {
                object.draw(input, model);
            }
            for (std::shared_ptr<const GLNode> child : children)
            {
                child->draw(input, model);
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
        std::shared_ptr<GLShadowCubeRenderTarget> point_light_shadow_render_target = nullptr;
        std::shared_ptr<GLShadowRenderTarget> directional_light_shadow_render_target = nullptr;
        
    public:
        
        void add_light(std::shared_ptr<GLPointLight> light)
        {
            if(!point_light_shadow_render_target)
            {
                point_light_shadow_render_target = std::make_shared<GLShadowCubeRenderTarget>(1024, 1024);
            }
            point_lights.push_back(light);
        }
        
        void add_light(std::shared_ptr<GLDirectionalLight> light)
        {
            if(!directional_light_shadow_render_target)
            {
                directional_light_shadow_render_target = std::make_shared<GLShadowRenderTarget>(1024, 1024);
            }
            directional_lights.push_back(light);
        }
        
        void draw(DrawPassInput& input) const
        {
            GLNode::draw(input, glm::mat4(1.));
        }
        
        void shine_light(const glm::vec3& view_position, const glm::vec3& view_direction,
                         const GLColorRenderTarget& source_render_target, const GLRenderTarget& render_target) const
        {
            // Set up blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            
            for(auto light : point_lights)
            {
                // Cast shadows
                point_light_shadow_render_target->use();
                for (int i = 0; i < 6; i++)
                {
                    point_light_shadow_render_target->bind_texture_for_writing(i);
                    point_light_shadow_render_target->clear();
                    
                    auto input = DrawPassInput();
                    input.mode = DEFERRED;
                    input.camera_position = view_position;
                    input.view = light->get_view(i);
                    input.projection = light->get_projection();
                    
                    draw(input);
                }
                
                // Shine the light
                render_target.use();
                light->shine(view_position, source_render_target, *point_light_shadow_render_target);
            }
            
            for(auto light : directional_lights)
            {
                // Cast shadows
                directional_light_shadow_render_target->use();
                directional_light_shadow_render_target->clear();
                
                auto input = DrawPassInput();
                input.mode = DEFERRED;
                input.camera_position = view_position;
                input.view = light->get_view();
                input.projection = light->get_projection();
                
                draw(input);
                
                // Shine the light
                render_target.use();
                light->shine(view_position, view_direction, source_render_target, *directional_light_shadow_render_target);
            }
        }
    };
    
    class GLSwitchNode : public GLNode
    {
    public:
        GLSwitchNode(const std::shared_ptr<bool> _enabled) : enabled(_enabled)
        {
            
        }
        
    private:
        std::shared_ptr<bool> enabled;
        
        void draw(DrawPassInput& input, const glm::mat4& model) const
        {
            if(*enabled)
            {
                GLNode::draw(input, model);
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
        
        void draw(DrawPassInput& input, const glm::mat4& model) const
        {
            GLNode::draw(input, model * (*transformation));
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
        
        void draw(DrawPassInput& input, const glm::mat4& model) const
        {
            GLNode::draw(input, model * glm::rotate(glm::mat4(1.f), *angle, axis));
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
        
        void draw(DrawPassInput& input, const glm::mat4& model) const
        {
            GLNode::draw(input, model * glm::translate(glm::mat4(1.f), *translation));
        }
    };
    
    class GLScaleNode : public GLNode
    {
    public:
        GLScaleNode(double _scale) : scale(std::make_shared<glm::vec3>(_scale, _scale, _scale))
        {
            
        }
        
        GLScaleNode(const glm::vec3& _scale) : scale(std::make_shared<glm::vec3>(_scale))
        {
            
        }
        
        GLScaleNode(std::shared_ptr<glm::vec3> _scale) : scale(_scale)
        {
            
        }
        
    private:
        std::shared_ptr<glm::vec3> scale;
        
        void draw(DrawPassInput& input, const glm::mat4& model) const
        {
            GLNode::draw(input, model * glm::scale(glm::mat4(1.f), *scale));
        }
    };

}
