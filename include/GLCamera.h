//
//  Created by Asger Nyman Christiansen on 03/10/15.
//  Copyright (c) 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLScene.h"

namespace oogl {
    
    /**
     Represents a camera.
     */
    class GLCamera
    {
        glm::vec3 position = glm::vec3(0.);
        glm::mat4 view = glm::mat4(1.);
        glm::mat4 projection = glm::mat4(1.);
        
    public:
        
        GLCamera()
        {
            // Enable states
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            
            glEnable(GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        /**
         Reshape the window.
         */
        void set_screen_size(int width, int height)
        {
            glViewport(0, 0, width, height);
            projection = glm::perspective(45.f, width/float(height), 0.1f, 100.f);
        }
        
        /**
         Set the camera/eye.
         */
        void set_view(const glm::vec3& eyePosition, const glm::vec3& eyeDirection)
        {
            position = eyePosition;
            view = lookAt(eyePosition, eyePosition + eyeDirection, glm::vec3(0., 1., 0.));
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
            glDepthMask(GL_TRUE);
            
            glClearColor(1., 1., 1., 0.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            scene.draw(position, view, projection);
            
            check_gl_error();
        }
    };
}
