//
//  Created by Asger Nyman Christiansen on 12/05/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "GLMaterial.h"

class GLSandMaterial : public GLMaterial
{
    std::shared_ptr<mesh::Attribute<mesh::VertexID, float>> sand_density;
    std::unique_ptr<gle::GLTexture2D> noise_texture;
public:
    
    GLSandMaterial(std::shared_ptr<mesh::Attribute<mesh::VertexID, float>> _sand_density)
    : GLMaterial(FORWARD, "shaders/sand.vert",  "shaders/sand.frag"), sand_density(_sand_density)
    {
        create_noise_texture();
    }
    
    void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<gle::GLVertexAttribute<float>>>& float_vertex_attributes)
    {
        float_vertex_attributes.push_back(GLVertexAttribute<float>::use(*shader, "density", sand_density));
    }
    
    void create_attributes(std::shared_ptr<mesh::Mesh> geometry, std::vector<std::shared_ptr<gle::GLVertexAttribute<glm::vec3>>>& vertex_attributes)
    {
        auto shader = get_shader();
        GLMaterial::create_attributes(geometry, vertex_attributes);
        vertex_attributes.push_back(GLVertexAttribute<glm::vec3>::use(*shader, "normal", geometry->normal()));
    }
    
    void pre_draw(const DrawPassInput& input, const glm::mat4& model)
    {
        auto shader = get_shader();
        gle::GLState::depth_test(true);
        gle::GLState::depth_write(true);
        gle::GLState::cull_back_faces(true);
        
        noise_texture->use(0);
        gle::GLUniform::use(shader, "noiseTexture", 0);
        
        gle::GLUniform::use(shader, "MMatrix", model);
        gle::GLUniform::use(shader, "MVPMatrix", input.projection * input.view * model);
        gle::GLUniform::use(shader, "NMatrix", inverseTranspose(model));
    }
    
private:
    void create_noise_texture()
    {
        const int noise_size = 128;
        auto noise = std::vector<float>(noise_size * noise_size);
        for (int i = 0; i < noise_size * noise_size; ++i)
        {
            noise[i] = gle::random(0., 1.);
        }
        noise_texture = std::unique_ptr<gle::GLTexture2D>(new gle::GLTexture2D(&noise[0], noise_size, noise_size, GL_RED));
    }
};
