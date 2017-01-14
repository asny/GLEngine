//
//  Created by Asger Nyman Christiansen on 30/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLObject.h"
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
        
    public:
        
        GLScene()
        {
            auto default_light = std::make_shared<GLDirectionalLight>(glm::vec3(-1., -1., -1.));
            lights.push_back(default_light);
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
        
        void shine_light(const glm::vec2& screen_size, const std::shared_ptr<GLTexture> position_texture, const std::shared_ptr<GLTexture> color_texture, const std::shared_ptr<GLTexture> normal_texture, const std::shared_ptr<GLTexture> depth_texture) const
        {
            for(auto light : lights)
            {
                light->shine(screen_size, position_texture, color_texture, normal_texture, depth_texture);
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
            GLNode::draw(draw_pass, camera_position, (*transformation) * model, view, projection);
        }
    };

}
