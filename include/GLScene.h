//
//  Created by Asger Nyman Christiansen on 30/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLObject.h"
#include "GLCamera.h"

namespace oogl {
    class GLScene
    {
        std::shared_ptr<glm::mat4> modelView = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> inverseModelView = std::make_shared<glm::mat4>(1.);
        std::shared_ptr<glm::mat4> modelViewProjection = std::make_shared<glm::mat4>(1.);
        
        std::shared_ptr<glm::vec3> light_pos = std::make_shared<glm::vec3>(0., 2000., 2.);
        
    public:
        GLScene(std::shared_ptr<GLCamera> _camera) : camera(_camera)
        {
            
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
        
        void add(std::shared_ptr<GLObject> object)
        {
            objects.push_back(object);
            object->get_material()->setup_camera(camera->get_view(), modelView, inverseModelView, camera->get_projection(), modelViewProjection, camera->get_position());
            object->get_material()->setup_light(light_pos);
        }
        
        void draw()
        {
            camera->pre_draw();
            
            for (std::shared_ptr<GLObject> object : objects)
            {
                *modelView = *camera->get_view() * object->get_model();
                *inverseModelView = inverseTranspose(*modelView);
                *modelViewProjection = *camera->get_projection() * (*modelView);
                
                object->draw();
            }
            
            check_gl_error();
        }
        
    private:
        std::vector<std::shared_ptr<GLObject>> objects = std::vector<std::shared_ptr<GLObject>>();
        std::shared_ptr<GLCamera> camera;
    };
}
