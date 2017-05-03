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
        int width;
        int height;
        
        glm::vec3 position = glm::vec3(0.);
        glm::vec3 direction = glm::vec3(0., 0., -1.);
        glm::mat4 view = glm::mat4(1.);
        glm::mat4 projection = glm::mat4(1.);
        
        std::shared_ptr<GLScreenRenderTarget> screen_render_target;
        std::shared_ptr<GLColorRenderTarget> geometry_pass_render_target;
        
    public:
        
        GLCamera(int screen_width, int screen_height)
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
            screen_render_target = std::make_shared<GLScreenRenderTarget>(width, height);
            geometry_pass_render_target = std::make_shared<GLColorRenderTarget>(width, height, 3, true);
            projection = glm::perspective(glm::radians(45.f), width/float(height), z_near, z_far);
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
        
        void draw(const GLScene& scene)
        {
            deferred_pass(scene);
            forward_pass(scene);
            
            check_gl_error();
        }
        
        void apply_post_effect(const GLPostEffect& post_effect)
        {
            screen_render_target->use();
            
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            post_effect.apply(*geometry_pass_render_target, position, view, projection);
        }
        
        const glm::vec3& get_position()
        {
            return position;
        }
        
        const glm::vec3& get_direction()
        {
            return direction;
        }
        
        glm::vec3 get_view_direction_at(int screen_coord_x, int screen_coord_y)
        {
            glm::vec4 screen_pos = glm::vec4(2. * static_cast<double>(screen_coord_x) / static_cast<double>(width) - 1.,
                                             1.-(2. * static_cast<double>(screen_coord_y) / static_cast<double>(height)), 1., 1.);
            glm::vec4 ray_eye = glm::inverse(projection) * screen_pos;
            ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
            glm::vec4 ray_world = inverse(view) * ray_eye;
            return glm::normalize(glm::vec3(ray_world.x, ray_world.y, ray_world.z));
        }
        
    private:
        void forward_pass(const GLScene& scene)
        {
            screen_render_target->use();
            
            // Set up default blending
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // Draw the scene
            scene.draw(FORWARD, position, view, projection);
        }
        
        void deferred_pass(const GLScene& scene)
        {
            // Geometry pass
            geometry_pass_render_target->use();
            geometry_pass_render_target->clear();
            
            glDisable(GL_BLEND);
            
            scene.draw(DEFERRED, position, view, projection);
            
            // Light pass
            screen_render_target->use();
            screen_render_target->clear();
            
            scene.shine_light(position, direction, *geometry_pass_render_target, *screen_render_target);
        }
    };
}
