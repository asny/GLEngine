//
//  Created by Asger Nyman Christiansen on 30/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLObject.h"

namespace oogl
{
    class GLNode
    {
    public:
        
        void add_leaf(std::shared_ptr<geogo::Mesh> geometry, std::shared_ptr<GLMaterial> material)
        {
            auto object = GLObject(geometry, material);
            objects.push_back(object);
        }
        
        void add_child(std::shared_ptr<GLNode> node)
        {
            nodes.push_back(node);
        }
        
    protected:
        std::vector<const GLObject> objects = std::vector<const GLObject>();
        std::vector<std::shared_ptr<GLNode>> nodes = std::vector<std::shared_ptr<GLNode>>();
        
        virtual void draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            for (const GLObject& object : objects)
            {
                object.draw(light_pos, camera_position, model, view, projection);
            }
            for (std::shared_ptr<GLNode> node : nodes)
            {
                node->draw(camera_position, model, view, projection);
            }
        }
        glm::vec3 light_pos = glm::vec3(5., 5., 5.);
    };
    
    class GLTransformationNode : public GLNode
    {
    public:
        GLTransformationNode(const glm::mat4& _transformation)
        {
            transformation = std::make_shared<glm::mat4>(_transformation);
        }
        
        GLTransformationNode(const std::shared_ptr<glm::mat4> _transformation)
        {
            transformation = _transformation;
        }
        
    private:
        std::shared_ptr<glm::mat4> transformation;
        
        void draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            GLNode::draw(camera_position, (*transformation) * model, view, projection);
        }
    };

    class GLScene : public GLNode
    {
    public:
        GLScene()
        {
            
        }
        
        void draw(const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            auto model = glm::mat4(1.);
            GLNode::draw(camera_position, model, view, projection);
        }
        
    private:
    };

}
