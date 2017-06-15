//
//  Created by Asger Nyman Christiansen on 09/08/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"
#include "GLShader.h"
#include "Mesh.h"

namespace gle
{   
    template<class ValueType>
    class GLVertexAttribute
    {
    public:
        static std::shared_ptr<GLVertexAttribute<ValueType>> use(GLShader& shader, std::string name, std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>> attribute)
        {
            auto glAttribute = std::make_shared<GLVertexAttribute<ValueType>>(attribute);
            
            // Initialize attribute
            auto size = static_cast<int>(sizeof(ValueType) / sizeof(float));
            auto location = shader.get_attribute_location(name);
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (const GLvoid *)(0));
            check_gl_error();
            
            return glAttribute;
        }
        
        GLVertexAttribute(std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>> _attribute) : attribute(_attribute)
        {
            // Generate and bind buffer
            glGenBuffers(1, &buffer_id);
            glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            
            size = sizeof(ValueType) / sizeof(float);
            
            std::function<void()> on_attribute_changed = std::bind(&GLVertexAttribute::deprecate, this);
            _attribute->listen_to(on_attribute_changed);
        }
        
        bool is_up_to_date()
        {
            return up_to_date;
        }
        
        void add_data_at(const mesh::VertexID& id)
        {
            add_data_at(id, attribute->at(id));
        }
        
        void send_data()
        {
            // Bind buffer and send data
            glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
            check_gl_error();
            
            up_to_date = true;
            data.clear();
        }
        
    private:
        
        void add_data_at(const mesh::VertexID& id, float value)
        {
            data.push_back(value);
        }
        
        void add_data_at(const mesh::VertexID& id, glm::vec2 value)
        {
            data.push_back(value[0]);
            data.push_back(value[1]);
        }
        
        void add_data_at(const mesh::VertexID& id, glm::vec3 value)
        {
            data.push_back(value[0]);
            data.push_back(value[1]);
            data.push_back(value[2]);
        }
        
        void deprecate()
        {
            up_to_date = false;
        }
        
        bool up_to_date = false;
        std::shared_ptr<const mesh::Attribute<mesh::VertexID, ValueType>> attribute;
        GLuint buffer_id;
        int size;
        std::vector<float> data;
    };
}
