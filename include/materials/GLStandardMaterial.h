//
//  GLStandardMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace oogl
{
    class GLStandardMaterial : public GLMaterial
    {
        const std::shared_ptr<geogo::Attribute<geogo::VertexID, glm::vec3>> normals;
    public:
        
        GLStandardMaterial(const std::shared_ptr<geogo::Attribute<geogo::VertexID, glm::vec3>> _normals, const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular, double _opacity) : normals(_normals)
        {
            shader = GLShader::create_or_get("shaders/phong.vert",  "shaders/phong.frag");
            use_uniform("ambientMat", _ambient);
            use_uniform("diffuseMat", _diffuse);
            use_uniform("specMat", _specular);
            use_uniform("opacity", _opacity);
            
            test_depth = _opacity >= 0.999;
        }
        
        void create_attributes(std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(create_attribute("normal", normals));
        }
    };
}
