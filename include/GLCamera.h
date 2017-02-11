//
//  Created by Asger Nyman Christiansen on 03/10/15.
//  Copyright (c) 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLScene.h"
#include "GLRenderTarget.h"

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
        
        GLRenderTarget deferred_render_target;
        GLRenderTarget shadow_render_target;
        
    public:
        
        GLCamera(int screen_width, int screen_height) :
            deferred_render_target(GLRenderTarget(screen_width, screen_height, 3, true)),
            shadow_render_target(GLRenderTarget(screen_width, screen_height, 0, true))
        {
            set_screen_size(screen_width, screen_height);
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
            deferred_render_target.use();
            
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
            shadow_render_target.use();
            
            // Clear the buffer
            GLState::depth_write(true);
            glClear(GL_DEPTH_BUFFER_BIT);
            
            // Draw the scene
            glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10,10,-10,10,-10,20);
            glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(-5., 5., 0.), glm::vec3(0,0,0), glm::vec3(0,1,0));
            scene.draw(SHADOW, position, depthViewMatrix, depthProjectionMatrix);
            
            // Bind buffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            // Set up blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            
            // Draw the scene
            glm::mat4 biasMatrix(
                                 0.5, 0.0, 0.0, 0.0,
                                 0.0, 0.5, 0.0, 0.0,
                                 0.0, 0.0, 0.5, 0.0,
                                 0.5, 0.5, 0.5, 1.0
                                 );
            scene.shine_light(glm::vec2(width, height), position, biasMatrix * depthProjectionMatrix * depthViewMatrix,
                              deferred_render_target.get_color_texture(0),
                              deferred_render_target.get_color_texture(1),
                              deferred_render_target.get_color_texture(2),
                              deferred_render_target.get_depth_texture(),
                              shadow_render_target.get_depth_texture());
        }
    };
}
