//
//  GLDeferredMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 27/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    
    class GLDeferredMaterial : public GLMaterial
    {
        const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> normals;
    public:
        
        GLDeferredMaterial(const std::shared_ptr<mesh::Attribute<mesh::VertexID, glm::vec3>> _normals) : normals(_normals)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/geometry_pass.vert",  "../GLEngine/shaders/geometry_pass.frag");
            
            use_uniform("MVPMatrix", modelViewProjection);
            use_uniform("MVMatrix", modelView);
            use_uniform("NMatrix", inverseModelView);
        }
        
        void create_attributes(std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(create_attribute("normal", normals));
        }
    };
}
