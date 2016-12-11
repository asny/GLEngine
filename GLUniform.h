//
//  Created by Asger Nyman Christiansen on 23/08/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"

namespace oogl
{
    template <class ValueType>
    class GLUniform
    {
    public:
        
        GLUniform(GLuint _location, const ValueType& _value) : value(std::make_shared<ValueType>(_value)), location(_location)
        {
            
        }
        
        GLUniform(GLuint _location, const std::shared_ptr<ValueType> _value) : value(_value), location(_location)
        {
            
        }
        
        void use()
        {
            set(*value);
        }
        
    private:
        const std::shared_ptr<ValueType> value;
        GLuint location;
        
        void set(float value)
        {
            glUniform1f(location, value);
        }
        
        void set(int value)
        {
            glUniform1i(location, value);
        }
        
        void set(const glm::vec2& value)
        {
            glUniform2fv(location, 1, &value[0]);
        }
        
        void set(const glm::vec3& value)
        {
            glUniform3fv(location, 1, &value[0]);
        }
        
        void set(const glm::vec4& value)
        {
            glUniform4fv(location, 1, &value[0]);
        }
        
        void set(const glm::mat4& value)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }
        
    };
}
