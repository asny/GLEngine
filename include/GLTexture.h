//
//  Created by Asger Nyman Christiansen on 06/11/15.
//  Copyright © 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"

namespace gle
{
    /**
     Represents an OpenGL texture
     */
    class GLTexture
    {
    protected:
        GLuint texture_id;
        GLenum target;
        GLenum minMagFilter = GL_LINEAR; // GL_NEAREST or GL_LINEAR
        GLenum wrapMode = GL_CLAMP_TO_EDGE; // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, or GL_CLAMP_TO_BORDER
        
        void bind_image(unsigned char* data, unsigned int width, unsigned int height, GLenum format, GLenum target)
        {
            glTexImage2D(target,
                         0,
                         format,
                         (GLsizei)width,
                         (GLsizei)height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         data);
        }
        
        /**
         Creates a texture from a bitmap.
         */
        GLTexture(GLenum _target) : target(_target)
        {
            glGenTextures(1, &texture_id);
            check_gl_error();
        }
        
        /**
         Deletes the texture.
         */
        ~GLTexture()
        {
            glDeleteTextures(1, &texture_id);
            check_gl_error();
        }
        
        void bind()
        {
            glBindTexture(target, texture_id);
        }
        
    public:
        /**
         Bind the texture to the given location.
         */
        void use(int location)
        {
            glActiveTexture(GL_TEXTURE0 + location);
            bind();
            check_gl_error();
        }
    };
    
    /**
     Represents an OpenGL texture
     */
    class GLTexture2D : public GLTexture
    {
    public:
        /**
         Creates a texture from a bitmap.
         */
        GLTexture2D(unsigned char* data, unsigned int width, unsigned int height, GLenum format) : GLTexture(GL_TEXTURE_2D)
        {
            bind();
            bind_image(data, width, height, format, GL_TEXTURE_2D);
            
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
            
            glBindTexture(GL_TEXTURE_2D, 0);
            check_gl_error();
        }
    };
    
    /**
     Represents an OpenGL cubemap texture
     */
    class GLTexture3D : public GLTexture
    {
    public:
        /**
         Creates a 3D texture from a set of bitmaps.
         */
        GLTexture3D(const std::vector<unsigned char*>& data, unsigned int width, unsigned int height, GLenum format) : GLTexture(GL_TEXTURE_CUBE_MAP)
        {
            bind();
            for(GLuint i = 0; i < data.size(); i++)
            {
                bind_image(data[i], width, height, format, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
            }
            
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapMode);
            
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            check_gl_error();
        }
    };
    
    /**
     Represents an OpenGL framebuffer texture
     */
    class GLFramebufferTexture : public GLTexture
    {
    public:
        GLFramebufferTexture(unsigned int width, unsigned int height, int channel) : GLTexture(GL_TEXTURE_2D)
        {
            bind();
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            
            glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + channel, GL_TEXTURE_2D, texture_id, 0);
            
            glBindTexture(GL_TEXTURE_2D, 0);
            check_gl_error();
        }
    };

}
