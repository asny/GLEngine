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
        std::shared_ptr<glm::vec3> position = std::make_shared<glm::vec3>();
        std::shared_ptr<glm::mat4> view = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> projection = std::make_shared<glm::mat4>(1.);
        
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
            *projection = glm::perspective(45.f, width/float(height), 0.1f, 100.f);
        }
        
        /**
         Set the camera/eye.
         */
        void set_view(const glm::vec3& eyePosition, const glm::vec3& eyeDirection)
        {
            *position = eyePosition;
            *view = lookAt(eyePosition, eyePosition + eyeDirection, glm::vec3(0., 1., 0.));
        }
        
        void draw(const GLScene& scene)
        {
            glDepthMask(GL_TRUE);
            
            glClearColor(1., 1., 1., 0.);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            scene.draw(*position, *view, *projection);
        }
        
        const std::shared_ptr<glm::mat4> get_view()
        {
            return view;
        }
        
        const std::shared_ptr<glm::mat4> get_projection()
        {
            return projection;
        }
        
        const std::shared_ptr<glm::vec3> get_position()
        {
            return position;
        }
        
    };
}
