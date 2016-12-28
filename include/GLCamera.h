//
//  Created by Asger Nyman Christiansen on 03/10/15.
//  Copyright (c) 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLScene.h"
#include "GLBuffer.h"

#include "materials/GLDeferredMaterial.h"
#include "MeshCreator.h"

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
        
        GBuffer buffer;
        GLObject screen_quad_object;
        
    public:
        
        GLCamera(int screen_width, int screen_height) : screen_quad_object(MeshCreator::create_quad(), std::make_shared<GLLightMaterial>())
        {
            // Enable states
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            set_screen_size(screen_width, screen_height);
            
            buffer.Init(screen_width, screen_height);
        }
        
        /**
         Reshape the window.
         */
        void set_screen_size(int _width, int _height)
        {
            width = _width;
            height = _height;
            projection = glm::perspective(45.f, width/float(height), 0.1f, 100.f);
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
            glDepthMask(GL_TRUE); // If it is not possible to write to the depth buffer, we are not able to clear it.
            glClearColor(1., 1., 1., 0.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        void draw(const GLScene& scene)
        {
            glViewport(x, y, width, height);
            scene.draw(position, view, projection);
            check_gl_error();
        }
        
        void draw_deferred(const GLScene& scene)
        {
            // Geometry pass
            glViewport(x, y, width, height);
            
            buffer.BindForWriting();
            clear_screen();
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_BLEND);
            
            scene.draw(position, view, projection);
            
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);
            
            // Light pass
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_ONE, GL_ONE);
            
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            buffer.BindForReading();
            glClearColor(0., 0., 0., 0.);
            glClear(GL_COLOR_BUFFER_BIT);
            
            screen_quad_object.draw(glm::vec3(0.), position, glm::mat4(1.), glm::mat4(1.), glm::mat4(1.));
            
            check_gl_error();
        }
        
        void draw_deferred_debug(const GLScene& scene)
        {
            glViewport(x, y, width, height);
            
            buffer.BindForWriting();
            clear_screen();
            
            scene.draw(position, view, projection);
            
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            buffer.BindForReadingDebug();
            
            GLint HalfWidth = (GLint)(width / 2.0f);
            GLint HalfHeight = (GLint)(height / 2.0f);
            
            buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
            glBlitFramebuffer(0, 0, width, height, 0, HalfHeight, HalfWidth, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

            buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
            glBlitFramebuffer(0, 0, width, height, HalfWidth, HalfHeight, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

            buffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
            glBlitFramebuffer(0, 0, width, height, HalfWidth, 0, width, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
            
            // Draw actual scene
            
            glViewport(0, 0, HalfWidth, HalfHeight);
            scene.draw(position, view, projection);
            
            check_gl_error();
        }
    };
}
