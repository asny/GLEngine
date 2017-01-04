//
//  Created by Asger Nyman Christiansen on 23/08/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"
#include "GLShader.h"

namespace gle
{
    template <class ValueType>
    class GLUniform
    {
    public:
        
        GLUniform(GLuint _location) : location(_location)
        {
            
        }
        
        static void use(std::shared_ptr<GLShader> shader, const std::string& name, const ValueType& value)
        {
            static std::map<std::string, std::shared_ptr<GLUniform>> dictionary = std::map<std::string, std::shared_ptr<GLUniform>>();
            auto key = shader->get_name() + name;
            auto iterator = dictionary.find(key);
            if (iterator == dictionary.end())
            {
                auto location = shader->get_uniform_location(name);
                auto uniform = std::make_shared<GLUniform<ValueType>>(location);
                iterator = dictionary.insert(std::make_pair(key, uniform)).first;
            }
            
            shader->use();
            iterator->second->set(value);
        }
        
    private:
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
