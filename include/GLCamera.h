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
        
        GLRenderTarget deferred_render_target;
        
    public:
        
        GLCamera(int screen_width, int screen_height) :
            deferred_render_target(GLRenderTarget(screen_width, screen_height, 3, true))
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
            GLRenderTarget::use_default();
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
            // Use default render target
            GLRenderTarget::use_default(false);
            
            // Set up default blending
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Draw the scene
            scene.draw(FORWARD, position, view, projection);
        }
        
        void geometry_pass(const GLScene& scene)
        {
            // Use deferred render target
            deferred_render_target.use();
            
            // Do not blend
            glDisable(GL_BLEND);
            
            // Draw the scene
            scene.draw(DEFERRED, position, view, projection);
        }
        
        void light_pass(const GLScene& scene)
        {
            scene.shine_light(glm::vec2(width, height), position,
                              deferred_render_target.get_color_texture(0),
                              deferred_render_target.get_color_texture(1),
                              deferred_render_target.get_color_texture(2),
                              deferred_render_target.get_depth_texture());
        }
    };
}
