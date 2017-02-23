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
        const float z_near = 0.1f;
        const float z_far = 100.f;
        
        glm::vec3 position = glm::vec3(0.);
        glm::vec3 direction = glm::vec3(0., 0., -1.);
        glm::mat4 view = glm::mat4(1.);
        glm::mat4 projection = glm::mat4(1.);
        
        GLRenderTarget geometry_pass_render_target, light_pass_render_target;
        std::shared_ptr<GLPostEffect> post_effect = nullptr;
        
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
            projection = glm::perspective(45.f, width/float(height), z_near, z_far);
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
            auto render_target = post_effect ? light_pass_render_target : GLDefaultRenderTarget::get();
            
            deferred_pass(scene, render_target);
            forward_pass(scene, render_target);
            
            if(post_effect)
            {
                post_effect_pass();
            }
            
            check_gl_error();
        }
        
        void set_post_effect(std::shared_ptr<GLPostEffect> _post_effect)
        {
            post_effect = _post_effect;
        }
        
    private:
        void forward_pass(const GLScene& scene, const GLDefaultRenderTarget& render_target)
        {
            render_target.use();
            
            // Set up default blending
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Draw the scene
            scene.draw(FORWARD, position, view, projection);
        }
        
        void deferred_pass(const GLScene& scene, const GLDefaultRenderTarget& render_target)
        {
            // Geometry pass
            geometry_pass_render_target.use();
            geometry_pass_render_target.clear();
            
            glDisable(GL_BLEND);
            
            scene.draw(DEFERRED, position, view, projection);
            
            // Light pass
            render_target.use();
            render_target.clear();
            
            scene.shine_light(position, direction, geometry_pass_render_target, render_target);
        }
        
        void post_effect_pass()
        {
            GLDefaultRenderTarget::get().use();
            GLDefaultRenderTarget::get().clear();
            
            post_effect->apply(light_pass_render_target, z_near, z_far);
        }
    };
}
