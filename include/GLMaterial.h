//
//  Created by Asger Nyman Christiansen on 27/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLTexture.h"
#include "GLUniform.h"

namespace gle
{
    enum DrawPassMode {FORWARD, DEFERRED};
    
    class GLMaterial
    {
    protected:
        
        bool cull_back_faces = true;
        bool test_depth = true;
        
        std::shared_ptr<GLShader> shader;
        
    public:
        
        virtual bool should_draw(DrawPassMode draw_pass) = 0;
        
        virtual void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
        }
        
        virtual void pre_draw(const glm::vec3& _camera_position, const glm::mat4& _model, const glm::mat4& _view, const glm::mat4& _projection) = 0;
        
        void pre_draw()
        {
            shader->use();
            
            // Cull back faces
            static bool currently_cull_back_faces = true;
            if(currently_cull_back_faces != cull_back_faces)
            {
                if(cull_back_faces)
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }
                else {
                    glDisable(GL_CULL_FACE);
                }
                currently_cull_back_faces = cull_back_faces;
            }
            
            // Depth test
            static bool currently_test_depth = true;
            if(currently_test_depth != test_depth)
            {
                if(test_depth)
                {
                    glDepthMask(GL_TRUE);
                }
                else {
                    glDepthMask(GL_FALSE);
                    
                }
                currently_test_depth = test_depth;
            }
        }
    };
}
