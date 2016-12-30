//
//  Created by Asger Nyman Christiansen on 30/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLObject.h"

namespace gle
{
    class GLNode
    {
        struct Object
        {
            std::shared_ptr<GLObject> geometry;
            std::shared_ptr<GLMaterial> material;
        };
    public:
        
        void add_leaf(std::shared_ptr<mesh::Mesh> geometry, std::shared_ptr<GLMaterial> material)
        {
            Object object = {std::make_shared<GLObject>(geometry, material), material};
            objects.push_back(object);
        }
        
        void add_child(std::shared_ptr<GLNode> node)
        {
            children.push_back(node);
        }
        
    protected:
        virtual void draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            for (const Object& object : objects)
            {
                object.material->pre_draw(camera_position, model, view, projection);
                object.geometry->draw();
            }
            for (std::shared_ptr<const GLNode> child : children)
            {
                child->draw(camera_position, model, view, projection);
            }
        }
        
    private:
        std::vector<const Object> objects = std::vector<const Object>();
        std::vector<std::shared_ptr<GLNode>> children = std::vector<std::shared_ptr<GLNode>>();
    };
    
    class GLScene : public GLNode
    {
    public:
        
        void draw(const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            auto model = glm::mat4(1.);
            GLNode::draw(camera_position, model, view, projection);
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
        
        void draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
        {
            GLNode::draw(camera_position, (*transformation) * model, view, projection);
        }
    };

}
