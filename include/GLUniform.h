//
//  Created by Asger Nyman Christiansen on 23/08/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLShader.h"

namespace gle
{
    class GLUniform
    {
    public:
        
        template<typename ValueType>
        static void use(std::shared_ptr<GLShader> shader, const std::string& name, const ValueType& value, int count = 1)
        {
            static std::map<std::string, GLuint> dictionary = std::map<std::string, GLuint>();
            auto key = shader->get_name() + name;
            auto iterator = dictionary.find(key);
            
            // Lazy construction
            if (iterator == dictionary.end())
            {
                auto location = shader->get_uniform_location(name);
                iterator = dictionary.insert(std::make_pair(key, location)).first;
            }
            
            // Use the value
            shader->use();
            GLUniform::set(iterator->second, value, count);
        }
        
    private:
        static void set(GLuint location, const float& value, int count)
        {
            glUniform1fv(location, count, &value);
        }
        
        static void set(GLuint location, const int& value, int count)
        {
            glUniform1iv(location, count, &value);
        }
        
        static void set(GLuint location, const glm::vec2& value, int count)
        {
            glUniform2fv(location, count, &value[0]);
        }
        
        static void set(GLuint location, const glm::vec3& value, int count)
        {
            glUniform3fv(location, count, &value[0]);
        }
        
        static void set(GLuint location, const glm::vec4& value, int count)
        {
            glUniform4fv(location, count, &value[0]);
        }
        
        static void set(GLuint location, const glm::mat4& value, int count)
        {
            glUniformMatrix4fv(location, count, GL_FALSE, &value[0][0]);
        }
    };
}
