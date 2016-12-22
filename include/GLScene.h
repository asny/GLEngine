//
//  Created by Asger Nyman Christiansen on 30/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLObject.h"

namespace oogl {
    class GLScene
    {
        
        glm::vec3 light_pos = glm::vec3(5., 5., 5.);
        
    public:
        GLScene()
        {
            
        }
        
        void add(std::shared_ptr<GLObject> object)
        {
            objects.push_back(object);
        }
        
        void draw(const glm::vec3& camera_position, const glm::mat4& view, const glm::mat4& projection) const
        {
            for (std::shared_ptr<GLObject> object : objects)
            {
                object->draw(light_pos, camera_position, object->get_model(), view, projection);
            }
        }
        
    private:
        std::vector<std::shared_ptr<GLObject>> objects = std::vector<std::shared_ptr<GLObject>>();
    };
}
