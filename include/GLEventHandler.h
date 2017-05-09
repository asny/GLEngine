//
//  Created by Asger Nyman Christiansen on 04/05/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLCamera.h"
#include "Search.h"
#include "effects/GLDebugEffect.h"

namespace gle
{
    class GLEventHandler
    {
    public:
        static void navigate_spherical(SDL_Event& e, GLCamera& camera)
        {
            static bool mouse_rotation = false;
            
            if( e.type == SDL_MOUSEWHEEL)
            {
                auto position = camera.get_position() + static_cast<float>(e.wheel.y) * camera.get_direction();
                camera.set_view(position, camera.get_direction());
            }
            if( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                mouse_rotation = true;
            }
            if( e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
            {
                mouse_rotation = false;
            }
            if(mouse_rotation && e.type == SDL_MOUSEMOTION)
            {
                glm::vec3 direction = camera.get_direction();
                glm::vec3 up_direction = glm::vec3(0., 1., 0.);
                glm::vec3 right_direction = glm::cross(direction, up_direction);
                up_direction = glm::cross(right_direction, direction);
                glm::vec3 camera_position = camera.get_position();
                float zoom = length(camera_position);
                camera_position += 0.1f * (static_cast<float>(e.motion.xrel) * right_direction + static_cast<float>(e.motion.yrel) * up_direction);
                camera_position = zoom * normalize(camera_position);
                camera.set_view(camera_position, glm::normalize(-camera_position));
            }
        }
        
        static void show_debug(SDL_Event& e, GLDebugEffect& debug_effect)
        {
            if( e.key.keysym.sym == SDLK_0)
            {
                debug_effect.type = gle::GLDebugEffect::NONE;
            }
            if( e.key.keysym.sym == SDLK_6)
            {
                debug_effect.type = gle::GLDebugEffect::POSITION;
            }
            if( e.key.keysym.sym == SDLK_7)
            {
                debug_effect.type = gle::GLDebugEffect::NORMAL;
            }
            if( e.key.keysym.sym == SDLK_8)
            {
                debug_effect.type = gle::GLDebugEffect::COLOR;
            }
            if( e.key.keysym.sym == SDLK_9)
            {
                debug_effect.type = gle::GLDebugEffect::DEPTH;
            }
        }
        
        static mesh::Search::Result picking(SDL_Event& e, const GLCamera& camera, const mesh::Mesh& model)
        {
            if( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                auto view_ray_origin = camera.get_position();
                auto view_ray_direction = camera.get_view_direction_at(e.button.x, e.button.y);
                return mesh::Search::closest_face(model, view_ray_origin, view_ray_direction);
            }
            return mesh::Search::Result();
        }
        
        static bool is_quitting(SDL_Event& e)
        {
            return e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE;
        }
    };
}
