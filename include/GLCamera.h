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
        
    public:
        
        GLCamera(int screen_width, int screen_height)
        {
            // Enable states
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glClearColor(1., 1., 1., 0.);
            
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
            glDepthMask(GL_TRUE); // If it is not possible to write to the depth buffer, we are not able to clear it.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        void draw(const GLScene& scene)
        {
            glViewport(x, y, width, height);
            scene.draw(position, view, projection);
            check_gl_error();
        }
    };
}
