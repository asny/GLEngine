//
//  Created by Asger Nyman Christiansen on 06/11/15.
//  Copyright © 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"
#include <SDL_image.h>

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
        GLenum wrapMode = GL_REPEAT; // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, or GL_CLAMP_TO_BORDER
        
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
        
        void bind_image(std::string filename, GLenum target, bool invert = true)
        {
            auto image = IMG_Load( filename.c_str() );
            if( image == NULL )
            {
                std::runtime_error("Unable to load image " + filename + "! SDL_image Error: " + IMG_GetError());
            }
            unsigned int width = image->w;
            unsigned int height = image->h;
            
            int bytesPerPixel = image->format->BytesPerPixel;
            if(bytesPerPixel != 3 && bytesPerPixel != 4)
            {
                std::runtime_error("Unknown image format.");
            }
            
            char *pixels = static_cast<char *>(image->pixels);
            
            if(invert)
            {
                invert_image(width * bytesPerPixel, height, pixels);
            }
            
            auto internal_format = bytesPerPixel == 3 ? GL_RGB : GL_RGBA;
            auto format = image->format->Rmask == 0x000000ff ? internal_format : (bytesPerPixel == 3 ? GL_BGR : GL_BGRA);
            glTexImage2D(target,
                         0,
                         (GLint) internal_format,
                         (GLsizei)width,
                         (GLsizei)height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         pixels);
            
            SDL_FreeSurface(image);
            check_gl_error();
        }
        
        /**
         Creates a texture from a bitmap.
         */
        GLTexture(GLenum _target) : target(_target)
        {
            glGenTextures(1, &texture_id);
        }
        
        /**
         Deletes the texture.
         */
        ~GLTexture()
        {
            glDeleteTextures(1, &texture_id);
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
        }
        
    private:
        
        static void invert_image(int width, int height, void *image_pixels)
        {
            auto temp_row = std::unique_ptr<char>(new char[width]);
            if (temp_row.get() == nullptr)
            {
                std::runtime_error("Not enough memory for image inversion");
            }
            //if height is odd, don't need to swap middle row
            int height_div_2 = height / 2;
            for (int index = 0; index < height_div_2; index++) {
                //uses string.h
                memcpy((Uint8 *)temp_row.get(),
                       (Uint8 *)(image_pixels)+
                       width * index,
                       width);
                
                memcpy(
                       (Uint8 *)(image_pixels)+
                       width * index,
                       (Uint8 *)(image_pixels)+
                       width * (height - index - 1),
                       width);
                memcpy(
                       (Uint8 *)(image_pixels)+
                       width * (height - index - 1),
                       temp_row.get(),
                       width);
            }
        }
    };
    
    /**
     Represents an OpenGL texture
     */
    class GLTexture2D : public GLTexture
    {
    public:
        GLTexture2D(std::string filename) : GLTexture(GL_TEXTURE_2D)
        {
            bind();
            bind_image(filename, GL_TEXTURE_2D);
            
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
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
        GLTexture3D(const std::vector<std::string>& filenames) : GLTexture(GL_TEXTURE_CUBE_MAP)
        {
            bind();
            for(GLuint i = 0; i < filenames.size(); i++)
            {
                bind_image(filenames[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, false);
            }
            
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, minMagFilter);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapMode);
            glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapMode);
        }
    };
    
    class GLFramebufferColorTexture : public GLTexture
    {
    public:
        GLFramebufferColorTexture(unsigned int width, unsigned int height, int channel) : GLTexture(GL_TEXTURE_2D)
        {
            bind();
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            
            glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + channel, GL_TEXTURE_2D, texture_id, 0);
        }
    };
    
    class GLFramebufferDepthTexture : public GLTexture
    {
    public:
        GLFramebufferDepthTexture(unsigned int width, unsigned int height) : GLTexture(GL_TEXTURE_2D)
        {
            bind();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            
            glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);
        }
    };
    
    class GLFramebufferDepthTextureCubeMap : public GLTexture
    {
    public:
        GLFramebufferDepthTextureCubeMap(unsigned int width, unsigned int height) : GLTexture(GL_TEXTURE_CUBE_MAP)
        {
            bind();
            for(GLuint i = 0; i < 6; i++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            }
            
            glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameterf(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            
            bind_for_writing(0);
        }
        
        void bind_for_writing(int layer)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + layer, texture_id, 0);
        }
    };
}
