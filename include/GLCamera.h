//
//  Created by Asger Nyman Christiansen on 03/10/15.
//  Copyright (c) 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLScene.h"

namespace gle {
    
    /**
     Represents a camera.
     */
    class GLCamera
    {
        glm::vec3 position = glm::vec3(0.);
        glm::mat4 view = glm::mat4(1.);
        glm::mat4 projection = glm::mat4(1.);
        
        int width;
        int height;
        int x = 0;
        int y = 0;
        
        GLuint framebufferobject_id;
        
        std::shared_ptr<GLTexture> position_texture;
        std::shared_ptr<GLTexture> color_texture;
        std::shared_ptr<GLTexture> normal_texture;
        std::shared_ptr<GLTexture> depth_texture;
        
    public:
        
        GLCamera(int screen_width, int screen_height)
        {
            glGenFramebuffers(1, &framebufferobject_id);
            
            set_screen_size(screen_width, screen_height);
        }
        
        ~GLCamera()
        {
            glDeleteFramebuffers(1, &framebufferobject_id);
        }
        
        /**
         Reshape the window.
         */
        void set_screen_size(int _width, int _height)
        {
            width = _width;
            height = _height;
            projection = glm::perspective(45.f, width/float(height), 0.1f, 100.f);
            resize_deferred_buffers();
        }
        
        void set_screen_position(int _x, int _y)
        {
            x = _x;
            y = _y;
        }
        
        /**
         Set the camera position and direction.
         */
        void set_view(const glm::vec3& _position, const glm::vec3& _direction)
        {
            position = _position;
            view = lookAt(position, _position + _direction, glm::vec3(0., 1., 0.));
        }
        
        void wireframe(bool on)
        {
            if(on)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        
        static void clear_screen()
        {
            GLState::depth_write(true); // If it is not possible to write to the depth buffer, we are not able to clear it.
            glClearColor(0., 0., 0., 0.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        void draw(const GLScene& scene)
        {
            glViewport(x, y, width, height);
            
            // Deffered draw
            geometry_pass(scene);
            light_pass(scene);
            
            // Forward draw
            forward_pass(scene);
            
            check_gl_error();
        }
    private:
        void forward_pass(const GLScene& scene)
        {
            // Set up default blending
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Draw the scene
            scene.draw(FORWARD, position, view, projection);
        }
        
        void geometry_pass(const GLScene& scene)
        {
            // Bind buffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
            
            // Do not blend
            glDisable(GL_BLEND);
            
            // Clear the buffer
            clear_screen();
            
            // Draw the scene
            scene.draw(DEFERRED, position, view, projection);
        }
        
        void light_pass(const GLScene& scene)
        {
            // Bind buffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            // Set up blending
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            
            // Draw the scene
            scene.shine_light(glm::vec2(width, height), position, position_texture, color_texture, normal_texture, depth_texture);
        }
        
        void resize_deferred_buffers()
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferobject_id);
            GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, DrawBuffers);
            
            // Create the textures
            position_texture = std::make_shared<GLFramebufferColorTexture>(width, height, 0);
            color_texture = std::make_shared<GLFramebufferColorTexture>(width, height, 1);
            normal_texture = std::make_shared<GLFramebufferColorTexture>(width, height, 2);
            depth_texture = std::make_shared<GLFramebufferDepthTexture>(width, height);
            
            GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            
            if (Status != GL_FRAMEBUFFER_COMPLETE) {
                printf("Framebuffer error, status: 0x%x\n", Status);
            }
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }
    };
}
