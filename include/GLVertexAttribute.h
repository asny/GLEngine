//
//  Created by Asger Nyman Christiansen on 09/08/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"
#include "Mesh.h"

namespace oogl
{   
    template<class ValueType>
    class GLVertexAttribute
    {
    public:
        GLVertexAttribute(std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>> _attribute) : attribute(_attribute)
        {
            // Generate and bind buffer
            glGenBuffers(1, &buffer_id);
            glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
            
            size = sizeof(ValueType) / sizeof(float);
            
            std::function<void()> on_attribute_changed = std::bind(&GLVertexAttribute::deprecate, this);
            _attribute->listen_to(on_attribute_changed);
        }
        
        static std::shared_ptr<GLVertexAttribute<ValueType>> create_or_get(std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>> _attribute)
        {
            auto iterator = dictionary.find(_attribute);
            if (iterator == dictionary.end())
            {
                auto glAttribute = std::make_shared<GLVertexAttribute<ValueType>>(_attribute);
                iterator = dictionary.insert(std::make_pair(_attribute, glAttribute)).first;
            }
            return iterator->second;
        }
        
        bool is_up_to_date()
        {
            return up_to_date;
        }
        
        void add_data_at(const mesh::VertexID& id)
        {
            ValueType vec = attribute->at(id);
            for(int j = 0; j < size; j++)
            {
                data.push_back(vec[j]);
            }
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
        
        void deprecate()
        {
            up_to_date = false;
        }
        
        static std::map<std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>>, std::shared_ptr<GLVertexAttribute<ValueType>>> dictionary;
        bool up_to_date = false;
        std::shared_ptr<const mesh::Attribute<mesh::VertexID, ValueType>> attribute;
        GLuint buffer_id;
        int size;
        std::vector<float> data;
    };
    
    template<class ValueType>
    std::map<std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>>, std::shared_ptr<GLVertexAttribute<ValueType>>> GLVertexAttribute<ValueType>::dictionary
    = std::map<std::shared_ptr<mesh::Attribute<mesh::VertexID, ValueType>>, std::shared_ptr<GLVertexAttribute<ValueType>>>();
}
