//
//  GLFlatMaterial.h
//  GLEngineExamples
//
//  Created by Asger Nyman Christiansen on 21/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

namespace gle
{
    
    class GLFlatColorMaterial : public GLMaterial
    {
        glm::vec3 color;
    public:
        
        GLFlatColorMaterial(const glm::vec3& _color) : color(_color)
        {
            shader = GLShader::create_or_get("../GLEngine/shaders/pre_geom.vert",  "../GLEngine/shaders/color_material.frag", "../GLEngine/shaders/flat.geom");
        }
        
        bool should_draw(DrawPassMode draw_pass)
        {
            return draw_pass == DEFERRED;
        }
        
        void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>>& vec2_vertex_attributes,
                               std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>>& vec3_vertex_attributes)
        {
            vec3_vertex_attributes.push_back(shader->create_attribute("position", geometry->position()));
        }
        
        void pre_draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
        {
            GLState::depth_test(true);
            GLState::depth_write(true);
            GLState::cull_back_faces(true);
            
            GLUniform::use(shader, "MVMatrix", view * model);
            GLUniform::use(shader, "PMatrix", projection);
            
            GLUniform::use(shader, "materialColor", color);
        }
    };
    
}
