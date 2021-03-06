//
//  GLRenderTarget.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 11/02/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once
#include "GLTexture.h"

namespace gle
{
    class GLRenderTarget
    {
        int width, height;
        GLenum framebufferobject_id = 0;
        
    protected:
        void check_framebuffer()
        {
            GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (Status != GL_FRAMEBUFFER_COMPLETE) {
                printf("Framebuffer error, status: 0x%x\n", Status);
            }
        }
        
        GLRenderTarget(int _width, int _height)
        {
            width = _width;
            height = _height;
        }
        
        void generate_framebuffer()
        {
            glGenFramebuffers(1, &framebufferobject_id);
        }
        
        ~GLRenderTarget()
        {
            if(framebufferobject_id != 0)
                glDeleteFramebuffers(1, &framebufferobject_id);
        }
        
    public:
        void use() const
        {
            static GLenum current_render_target = 0;
            if(current_render_target != framebufferobject_id)
            {
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
                glViewport(0, 0, width, height);
                current_render_target = framebufferobject_id;
            }
        }
        
        virtual void clear() const
        {
            GLState::depth_write(true); // If it is not possible to write to the depth buffer, we are not able to clear it.
            glClearColor(0., 0., 0., 0.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };
    
    class GLScreenRenderTarget : public GLRenderTarget
    {
    public:
        GLScreenRenderTarget(int _width, int _height) : GLRenderTarget(_width, _height)
        {
            
        }
    };
    
    class GLColorRenderTarget : public GLRenderTarget
    {
        std::vector<std::shared_ptr<GLTexture>> color_textures;
        std::shared_ptr<GLFramebufferDepthTexture> depth_texture;
        
    public:
        
        GLColorRenderTarget(int _width, int _height, int no_color_textures, bool create_depth_texture) : GLRenderTarget(_width, _height)
        {
            generate_framebuffer();
            use();
            
            GLenum DrawBuffers[no_color_textures];
            color_textures = std::vector<std::shared_ptr<GLTexture>>(no_color_textures);
            
            for (int i = 0; i < no_color_textures; i++)
            {
                DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
                color_textures[i] = std::make_shared<GLFramebufferColorTexture>(_width, _height, i);
            }
            
            glDrawBuffers(no_color_textures, DrawBuffers);
            
            if(create_depth_texture)
            {
                depth_texture = std::make_shared<GLFramebufferDepthTexture>(_width, _height);
            }
            
            check_framebuffer();
        }
        
        void clear() const
        {
            if(depth_texture != nullptr) {
                GLRenderTarget::clear();
            }
            else {
                glClearColor(0., 0., 0., 0.);
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }
        
        const GLTexture* get_color_texture_at(int location) const
        {
            return color_textures.at(location).get();
        }
        
        const GLTexture* get_depth_texture() const
        {
            return depth_texture.get();
        }
    };
    
    class GLShadowRenderTarget : public GLRenderTarget
    {
        std::shared_ptr<GLFramebufferDepthTexture> depth_texture;
    public:
        
        GLShadowRenderTarget(int _width, int _height) : GLRenderTarget(_width, _height)
        {
            generate_framebuffer();
            use();
            
            glDrawBuffer(GL_NONE);
            
            depth_texture = std::make_shared<GLFramebufferDepthTexture>(_width, _height);
            
            check_framebuffer();
        }
        
        void clear() const
        {
            GLState::depth_write(true); // If it is not possible to write to the depth buffer, we are not able to clear it.
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        
        void bind_texture_for_reading(int texture_location) const
        {
            depth_texture->use(texture_location);
        }
    };
    
    class GLShadowCubeRenderTarget : public GLRenderTarget
    {
        std::shared_ptr<GLFramebufferDepthTextureCubeMap> depth_texture_cubemap;
        
    public:
        
        GLShadowCubeRenderTarget(int _width, int _height) : GLRenderTarget(_width, _height)
        {
            generate_framebuffer();
            use();
            
            glDrawBuffer(GL_NONE);
            
            depth_texture_cubemap = std::make_shared<GLFramebufferDepthTextureCubeMap>(_width, _height);
            
            check_framebuffer();
        }
        
        void clear() const
        {
            GLState::depth_write(true); // If it is not possible to write to the depth buffer, we are not able to clear it.
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        
        void bind_texture_for_reading(int texture_location) const
        {
            depth_texture_cubemap->use(texture_location);
        }
        
        void bind_texture_for_writing(int layer) const
        {
            depth_texture_cubemap->bind_for_writing(layer);
        }
    };
}
