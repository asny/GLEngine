//
//  Created by Asger Nyman Christiansen on 03/10/15.
//  Copyright (c) 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLScene.h"
#include "GLPostEffect.h"

namespace gle {
    
    /**
     Represents a camera.
     */
    class GLCamera
    {
        glm::vec3 position = glm::vec3(0.);
        glm::vec3 direction = glm::vec3(0., 0., -1.);
        glm::mat4 view = glm::mat4(1.);
        glm::mat4 projection = glm::mat4(1.);
        
        GLRenderTarget geometry_pass_render_target, light_pass_render_target;
        GLPostEffect post_effect;
        
    public:
        
        GLCamera(int screen_width, int screen_height)
        {
            set_screen_size(screen_width, screen_height);
        }
        
        /**
         Reshape the window.
         */
        void set_screen_size(int width, int height)
        {
            GLDefaultRenderTarget::get().resize(width, height);
            geometry_pass_render_target.resize(width, height, 3, true);
            light_pass_render_target.resize(width, height, 1, true);
            projection = glm::perspective(45.f, width/float(height), 0.1f, 100.f);
        }
        
        /**
         Set the camera position and direction.
         */
        void set_view(const glm::vec3& _position, const glm::vec3& _direction)
        {
            position = _position;
            direction = _direction;
            view = lookAt(position, position + direction, glm::vec3(0., 1., 0.));
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
        
        void draw(const GLScene& scene)
        {
            deferred_pass(scene);
            forward_pass(scene);
            post_effect_pass(scene);
            
            check_gl_error();
        }
    private:
        void forward_pass(const GLScene& scene)
        {
            light_pass_render_target.use();
            
            // Set up default blending
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Draw the scene
            scene.draw(FORWARD, position, view, projection);
        }
        
        void deferred_pass(const GLScene& scene)
        {
            // Geometry pass
            geometry_pass_render_target.use();
            geometry_pass_render_target.clear();
            
            glDisable(GL_BLEND);
            
            scene.draw(DEFERRED, position, view, projection);
            
            // Light pass
            light_pass_render_target.use();
            light_pass_render_target.clear();
            
            scene.shine_light(position, direction, geometry_pass_render_target, light_pass_render_target);
        }
        
        void post_effect_pass(const GLScene& scene)
        {
            GLDefaultRenderTarget::get().use();
            GLDefaultRenderTarget::get().clear();
            
            post_effect.apply(light_pass_render_target);
        }
    };
}
