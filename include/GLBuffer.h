//
//  Created by Asger Nyman Christiansen on 27/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"

namespace gle {
    class GBuffer
    {
    public:
        
        GBuffer()
        {
            m_fbo = 0;
            m_depthTexture = 0;
        }
        
        ~GBuffer()
        {
            if (m_fbo != 0) {
                glDeleteFramebuffers(1, &m_fbo);
            }
            
            if (m_depthTexture != 0) {
                glDeleteTextures(1, &m_depthTexture);
            }
        }
        
        bool Init(unsigned int width, unsigned int height)
        {
            // Create the FBO
            glGenFramebuffers(1, &m_fbo);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
            
            // Create the textures
            position_texture = std::make_shared<GLFramebufferTexture>(width, height, 0);
            color_texture = std::make_shared<GLFramebufferTexture>(width, height, 1);
            normal_texture = std::make_shared<GLFramebufferTexture>(width, height, 2);
            
            // depth
            glGenTextures(1, &m_depthTexture);
            glBindTexture(GL_TEXTURE_2D, m_depthTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                         NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
            
            // Set the draw buffers
            GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, DrawBuffers);
            
            // Check status
            GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            
            if (Status != GL_FRAMEBUFFER_COMPLETE) {
                printf("FB error, status: 0x%x\n", Status);
                return false;
            }
            
            // restore default FBO
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            return true;
        }
        
        void BindForWriting()
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
        }
        
        void BindForReading()
        {
            position_texture->use(0);
            color_texture->use(1);
            normal_texture->use(2);
            check_gl_error();
        }
        
    private:
        
        GLuint m_fbo;
        
        std::shared_ptr<GLTexture> position_texture;
        std::shared_ptr<GLTexture> color_texture;
        std::shared_ptr<GLTexture> normal_texture;
        GLuint m_depthTexture;
    };
}
