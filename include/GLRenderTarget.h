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
        
        GLenum framebufferobject_id;
        std::vector<std::shared_ptr<GLTexture>> color_textures;
        std::shared_ptr<GLTexture> depth_texture;
    public:
        
        GLRenderTarget(int width, int height, int no_color_textures, bool create_depth_texture)
        {
            glGenFramebuffers(1, &framebufferobject_id);
            resize(width, height, no_color_textures, create_depth_texture);
        }
        
        ~GLRenderTarget()
        {
            glDeleteFramebuffers(1, &framebufferobject_id);
        }
        
        void resize(int width, int height, int no_color_textures, bool create_depth_texture)
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
            
            GLenum DrawBuffers[no_color_textures];
            color_textures = std::vector<std::shared_ptr<GLTexture>>(no_color_textures);
            
            for (int i = 0; i < no_color_textures; i++)
            {
                DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
                color_textures[i] = std::make_shared<GLFramebufferColorTexture>(width, height, i);
            }
            
            if(no_color_textures == 0)
                glDrawBuffer(GL_NONE);
            else
                glDrawBuffers(no_color_textures, DrawBuffers);
            
            
            if(create_depth_texture)
            {
                depth_texture = std::make_shared<GLFramebufferDepthTexture>(width, height);
            }
            
            GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if (Status != GL_FRAMEBUFFER_COMPLETE) {
                printf("Framebuffer error, status: 0x%x\n", Status);
            }
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }
        
        const std::shared_ptr<GLTexture> get_color_texture(int location) const
        {
            return color_textures.at(location);
        }
        
        const std::shared_ptr<GLTexture> get_depth_texture() const
        {
            return depth_texture;
        }
        
        void use() const
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
        }
    };
}
