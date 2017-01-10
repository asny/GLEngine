//
//  Created by Asger Nyman Christiansen on 27/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"

namespace gle
{
    class GLBuffer
    {
    public:
        
        GLBuffer(int width, int height)
        {
            // Create the FBO
            glGenFramebuffers(1, &framebufferobject_id);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
            
            // Create the textures
            position_texture = std::make_shared<GLFramebufferColorTexture>(width, height, 0);
            color_texture = std::make_shared<GLFramebufferColorTexture>(width, height, 1);
            normal_texture = std::make_shared<GLFramebufferColorTexture>(width, height, 2);
            depth_texture = std::make_shared<GLFramebufferDepthTexture>(width, height);
            
            // Set the draw buffers
            GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, DrawBuffers);
        }
        
        ~GLBuffer()
        {
            glDeleteFramebuffers(1, &framebufferobject_id);
        }
        
        void BindForWriting()
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
        }
        
        void BindForReading()
        {
            position_texture->use(0);
            color_texture->use(1);
            normal_texture->use(2);
        }
        
    private:
        
        GLuint framebufferobject_id;
        
        std::shared_ptr<GLTexture> position_texture;
        std::shared_ptr<GLTexture> color_texture;
        std::shared_ptr<GLTexture> normal_texture;
        std::shared_ptr<GLTexture> depth_texture;
    };
}
