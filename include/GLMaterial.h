//
//  Created by Asger Nyman Christiansen on 27/07/16.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLTexture.h"
#include "GLUniform.h"
#include "GLState.h"

namespace gle
{
    enum DrawPassMode {FORWARD, DEFERRED, SHADOW};
    
    class GLMaterial
    {
    protected:
        
        std::shared_ptr<GLShader> shader;
        
    public:
        
        virtual bool should_draw(DrawPassMode draw_pass) = 0;
        
        virtual void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                                       std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes) = 0;
        
        virtual void pre_draw(const glm::vec3& _camera_position, const glm::mat4& _model, const glm::mat4& _view, const glm::mat4& _projection) = 0;
    };
}
