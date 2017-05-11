//
//  Created by Asger Nyman Christiansen on 07/11/15.
//  Copyright © 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"
#include "GLVertexAttribute.h"

namespace gle {
    /**
     Represents a shader used to draw a GLObject.
     */
    class GLShader
    {
        std::string name;
        GLuint shader_id;
        
    public:
        GLShader(std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string geometryShaderFilename = "")
        {
            name = vertexShaderFilename + fragmentShaderFilename + geometryShaderFilename;
            if(geometryShaderFilename.length() != 0)
            {
                shader_id = init_shader(&vertexShaderFilename[0], &fragmentShaderFilename[0], &geometryShaderFilename[0]);
            }
            else {
                shader_id = init_shader(&vertexShaderFilename[0], &fragmentShaderFilename[0], nullptr);
            }
            
            check_gl_error();
        }
        
        const std::string& get_name()
        {
            return name;
        }
        
        static std::shared_ptr<GLShader> create_or_get(std::string vertexShaderFilename, std::string fragmentShaderFilename, std::string geometryShaderFilename = "")
        {
            static std::map<std::string, std::shared_ptr<GLShader>> dictionary = std::map<std::string, std::shared_ptr<GLShader>>();
            auto key = vertexShaderFilename + fragmentShaderFilename + geometryShaderFilename;
            auto iterator = dictionary.find(key);
            if (iterator == dictionary.end())
            {
                auto shader = std::make_shared<GLShader>(vertexShaderFilename, fragmentShaderFilename, geometryShaderFilename);
                iterator = dictionary.insert(std::make_pair(key, shader)).first;
            }
            return iterator->second;
        }
        
        void use()
        {
            static GLuint current_shader_id = -1;
            if(current_shader_id != shader_id)
            {
                glUseProgram(shader_id);
                current_shader_id = shader_id;
            }
        }
        
        // ******* Vertex attribute functionality ********
        
        GLuint get_attribute_location(std::string variable_name)
        {
            use();
            GLuint attributeLocation = glGetAttribLocation(shader_id, &variable_name[0]);
            if (attributeLocation == NULL_LOCATION) {
                std::cerr << "Shader did not contain the '" << variable_name << "' attribute variable."<<std::endl;
            }
            return attributeLocation;
        }
        
        std::shared_ptr<GLVertexAttribute<float>> create_attribute(std::string name, std::shared_ptr<mesh::Attribute<mesh::VertexID, float>> attribute)
        {
            auto glAttribute = std::make_shared<GLVertexAttribute<float>>(attribute);
            
            // Initialize attribute
            auto location = get_attribute_location(name);
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, sizeof(float), (const GLvoid *)(0));
            check_gl_error();
            
            return glAttribute;
        }
        
        std::shared_ptr<GLVertexAttribute<glm::vec2>> create_attribute(std::string name, std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec2>> attribute)
        {
            auto glAttribute = std::make_shared<GLVertexAttribute<glm::vec2>>(attribute);
            
            // Initialize attribute
            auto location = get_attribute_location(name);
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const GLvoid *)(0));
            check_gl_error();
            
            return glAttribute;
        }
        
        std::shared_ptr<GLVertexAttribute<glm::vec3>> create_attribute(std::string name, std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> attribute)
        {
            auto glAttribute = std::make_shared<GLVertexAttribute<glm::vec3>>(attribute);
            
            // Initialize attribute
            auto location = get_attribute_location(name);
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const GLvoid *)(0));
            check_gl_error();
            
            return glAttribute;
        }
        
        // ****** Uniform variable functionality ********
        
        GLuint get_uniform_location(std::string variable_name)
        {
            use();
            GLuint uniformLocation = glGetUniformLocation(shader_id, &variable_name[0]);
            if (uniformLocation == NULL_LOCATION) {
                std::cerr << "Shader did not contain the '" << variable_name << "' uniform variable."<<std::endl;
            }
            return uniformLocation;
        }
        
    private:
        // Create a GLSL program object from vertex and fragment shader files
        static GLuint init_shader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
        {
            struct Shader {
                std::string  filename;
                GLenum       type;
                GLchar*      source;
            };
            std::vector<Shader> shaders = { { vShaderFile, GL_VERTEX_SHADER, NULL }, { fShaderFile, GL_FRAGMENT_SHADER, NULL } };
            if(gShaderFile)
            {
                shaders.push_back({ gShaderFile, GL_GEOMETRY_SHADER, NULL });
            }
            
            GLuint program = glCreateProgram();
            
            for ( int i = 0; i < shaders.size(); ++i ) {
                Shader& s = shaders[i];
                s.source = read_shader_source( s.filename );
                if ( shaders[i].source == NULL ) {
                    std::cerr << "Failed to read " << s.filename << std::endl;
                    exit( EXIT_FAILURE );
                }
                GLuint shader = glCreateShader( s.type );
                glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
                glCompileShader( shader );
                
                GLint  compiled;
                glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
                if ( !compiled ) {
                    std::cerr << s.filename << " failed to compile:" << std::endl;
                    GLint  logSize;
                    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
                    char* logMsg = new char[logSize];
                    glGetShaderInfoLog( shader, logSize, NULL, logMsg );
                    std::cerr << logMsg << std::endl;
                    delete [] logMsg;
                    
                    exit( EXIT_FAILURE );
                }
                
                delete [] s.source;
                
                glAttachShader( program, shader );
            }
            
            /* link  and error check */
            glLinkProgram(program);
            
            GLint  linked;
            glGetProgramiv( program, GL_LINK_STATUS, &linked );
            if ( !linked ) {
                std::cerr << "Shader program failed to link" << std::endl;
                GLint  logSize;
                glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
                char* logMsg = new char[logSize];
                glGetProgramInfoLog( program, logSize, NULL, logMsg );
                std::cerr << logMsg << std::endl;
                delete [] logMsg;
                
                exit( EXIT_FAILURE );
            }
            
            return program;
        }
        
        static char* read_shader_source(std::string filePath)
        {
            FILE *filePointer;
            char *content = NULL;
            
            int count=0;
            
            if (filePath != "") {
                filePointer = fopen(filePath.c_str(),"rt");
                
                if (filePointer != NULL) {
                    
                    fseek(filePointer, 0, SEEK_END);
                    count = static_cast<int>(ftell(filePointer));
                    rewind(filePointer);
                    
                    if (count > 0) {
                        content = (char *)malloc(sizeof(char) * (count+1));
                        count = static_cast<int>(fread(content,sizeof(char),count,filePointer));
                        content[count] = '\0';
                    }
                    fclose(filePointer);
                }
            }
            return content;
        }
    };
}
