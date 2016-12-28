//
//  Created by Asger Nyman Christiansen on 27/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLUtility.h"

class GBuffer
{
public:
    
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
//        GBUFFER_TEXTURE_TYPE_DIFFUSE,
//        GBUFFER_TEXTURE_TYPE_NORMAL,
//        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };
    
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
        
        if (m_textures[0] != 0) {
            glDeleteTextures(GBUFFER_NUM_TEXTURES, m_textures);
        }
        
        if (m_depthTexture != 0) {
            glDeleteTextures(1, &m_depthTexture);
        }
    }
    
    bool Init(unsigned int WindowWidth, unsigned int WindowHeight)
    {
        // Create the FBO
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
        
        // Create the gbuffer textures
        glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
        glGenTextures(1, &m_depthTexture);
        
        for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES ; i++) {
            glBindTexture(GL_TEXTURE_2D, m_textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
        }
        
        // depth
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
        
        GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 };//, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);
        
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
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    }
    
    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
    }
    
private:
    
    GLuint m_fbo;
    GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_depthTexture;
};
