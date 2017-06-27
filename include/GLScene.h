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
            auto object = std::make_shared<GLObject>(geometry, material);
            objects.push_back(object);
            object->invalidate(geometry);
        }
        
        GLNode* add_child(std::shared_ptr<GLNode> node)
        {
            children.push_back(node);
            return node.get();
        }
        
        void remove_child(std::shared_ptr<GLNode> node)
        {
            auto pointer = std::find(children.begin(), children.end(), node);
            if(pointer == children.end())
                std::runtime_error("The node cannot be removed since it is not a child.");
            children.erase(pointer);
        }
        
        void invalidate(std::shared_ptr<mesh::Mesh> mesh)
        {
            for (auto object : objects)
            {
                object->invalidate(mesh);
            }
            for (auto child : children)
            {
                child->invalidate(mesh);
            }
        }
        
    protected:
        virtual void draw(DrawPassMode mode, const DrawPassInput& input, const glm::mat4& model)
        {
            for (auto object : objects)
            {
                if(object->should_draw_when(mode))
                {
                    object->draw(input, model);
                }
            }
            for (auto child : children)
            {
                child->draw(mode, input, model);
            }
        }
        
    private:
        std::vector<std::shared_ptr<GLObject>> objects = std::vector<std::shared_ptr<GLObject>>();
        std::vector<std::shared_ptr<GLNode>> children = std::vector<std::shared_ptr<GLNode>>();
    };
    
    class GLScene : public GLNode
    {
        std::vector<std::shared_ptr<GLDirectionalLight>> directional_lights;
        std::vector<std::shared_ptr<GLPointLight>> point_lights;
        std::shared_ptr<GLShadowCubeRenderTarget> point_light_shadow_render_target = nullptr;
        std::shared_ptr<GLShadowRenderTarget> directional_light_shadow_render_target = nullptr;
        
        const int shadow_width = 1024;
        const int shadow_height = 1024;
        
    public:
        
        void add_light(std::shared_ptr<GLPointLight> light)
        {
            if(!point_light_shadow_render_target)
            {
                point_light_shadow_render_target = std::make_shared<GLShadowCubeRenderTarget>(shadow_width, shadow_height);
            }
            point_lights.push_back(light);
        }
        
        void add_light(std::shared_ptr<GLDirectionalLight> light)
        {
            if(!directional_light_shadow_render_target)
            {
                directional_light_shadow_render_target = std::make_shared<GLShadowRenderTarget>(shadow_width, shadow_height);
            }
            directional_lights.push_back(light);
        }
        
        void draw(DrawPassMode mode, const DrawPassInput& input)
        {
            GLNode::draw(mode, input, glm::mat4(1.));
        }
        
        void shine_light(DrawPassMode mode, const DrawPassInput& input, const GLRenderTarget& render_target)
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
                    draw(mode, DrawPassInput(input.camera_position, glm::vec2(shadow_width, shadow_height), light->get_view(i), light->get_projection()));
                }
                
                // Shine the light
                render_target.use();
                light->shine(input, *point_light_shadow_render_target);
            }
            
            for(auto light : directional_lights)
            {
                // Cast shadows
                directional_light_shadow_render_target->use();
                directional_light_shadow_render_target->clear();
                draw(mode, DrawPassInput(input.camera_position, glm::vec2(shadow_width, shadow_height), light->get_view(), light->get_projection()));
                
                // Shine the light
                render_target.use();
                light->shine(input, *directional_light_shadow_render_target);
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
        
        void draw(DrawPassMode mode, const DrawPassInput& input, const glm::mat4& model)
        {
            if(*enabled)
            {
                GLNode::draw(mode, input, model);
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
        
        void draw(DrawPassMode mode, const DrawPassInput& input, const glm::mat4& model)
        {
            GLNode::draw(mode, input, model * (*transformation));
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
        
        void draw(DrawPassMode mode, const DrawPassInput& input, const glm::mat4& model)
        {
            GLNode::draw(mode, input, model * glm::rotate(glm::mat4(1.f), *angle, axis));
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
        
        void draw(DrawPassMode mode, const DrawPassInput& input, const glm::mat4& model)
        {
            GLNode::draw(mode, input, model * glm::translate(glm::mat4(1.f), *translation));
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
        
        void draw(DrawPassMode mode, const DrawPassInput& input, const glm::mat4& model)
        {
            GLNode::draw(mode, input, model * glm::scale(glm::mat4(1.f), *scale));
        }
    };

}
