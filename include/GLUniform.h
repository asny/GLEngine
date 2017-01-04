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
        static void use(std::shared_ptr<GLShader> shader, const std::string& name, const ValueType& value)
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
            GLUniform::set(iterator->second, value);
        }
        
    private:
        static void set(GLuint location, float value)
        {
            glUniform1f(location, value);
        }
        
        static void set(GLuint location, int value)
        {
            glUniform1i(location, value);
        }
        
        static void set(GLuint location, const glm::vec2& value)
        {
            glUniform2fv(location, 1, &value[0]);
        }
        
        static void set(GLuint location, const glm::vec3& value)
        {
            glUniform3fv(location, 1, &value[0]);
        }
        
        static void set(GLuint location, const glm::vec4& value)
        {
            glUniform4fv(location, 1, &value[0]);
        }
        
        static void set(GLuint location, const glm::mat4& value)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }
    };
}
