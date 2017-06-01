//
//  Created by Asger Nyman Christiansen on 07/11/15.
//  Copyright © 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "Mesh.h"
#include "MeshCreator.h"
#include "GLMaterial.h"
#include "GLVertexAttribute.h"
#include "GLUniform.h"

namespace gle
{
    /**
     Represents an object to draw.
     */
    class GLObject
    {
        std::shared_ptr<mesh::Mesh> geometry;
        std::shared_ptr<GLMaterial> material;
        
        GLuint array_id;
        std::vector<std::shared_ptr<GLVertexAttribute<float>>> float_vertex_attributes;
        std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>> vec2_vertex_attributes;
        std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>> vec3_vertex_attributes;
        
    public:
        
        GLObject(std::shared_ptr<mesh::Mesh> geometry, std::shared_ptr<GLMaterial> material) : material(material), geometry(geometry)
        {
            // Generate and bind array
            glGenVertexArrays(1, &array_id);
            glBindVertexArray(array_id);
            
            material->create_attributes(geometry, float_vertex_attributes);
            material->create_attributes(geometry, vec2_vertex_attributes);
            material->create_attributes(geometry, vec3_vertex_attributes);
        }
        
        void draw(const DrawPassInput& input, const glm::mat4& model) const
        {
            if(!material->should_draw(input.mode))
                return;
            
            // Use material specific uniforms and states
            material->pre_draw(input, model);
            draw();
        }
        
        static void draw_full_screen_quad(std::shared_ptr<GLShader> shader)
        {
            static GLuint array_id = NULL_LOCATION;
            if(array_id == NULL_LOCATION)
            {
                // Generate and bind array
                glGenVertexArrays(1, &array_id);
                glBindVertexArray(array_id);
                
                // Create mesh
                auto uv_coordinates = std::make_shared<mesh::Attribute<mesh::VertexID, glm::vec2>>();
                auto mesh = MeshCreator::create_quad(uv_coordinates);
                
                // Create attribute and send data.
                auto position = shader->create_attribute("position", mesh->position());
                update_attribute(mesh, position);
                
                auto uv = shader->create_attribute("uv_coordinates", uv_coordinates);
                update_attribute(mesh, uv);
            }
            glBindVertexArray(array_id);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
    private:
        
        void draw() const
        {
            // Infer draw mode
            GLenum drawmode;
            int no_vertices;
            if(geometry->get_no_faces() > 0)
            {
                drawmode = GL_TRIANGLES;
                no_vertices = geometry->get_no_faces() * 3;
            }
            else if(geometry->get_no_edges() > 0)
            {
                drawmode = GL_LINES;
                no_vertices = geometry->get_no_edges() * 2;
            }
            else if(geometry->get_no_vertices() > 0)
            {
                drawmode = GL_POINTS;
                no_vertices = geometry->get_no_vertices();
            }
            else
            {
                return;
            }
            
            // Update buffers if necessary
            for (auto glAttribute : float_vertex_attributes)
            {
                update_attribute(geometry, glAttribute);
            }
            
            for (auto glAttribute : vec2_vertex_attributes)
            {
                update_attribute(geometry, glAttribute);
            }
            
            for (auto glAttribute : vec3_vertex_attributes)
            {
                update_attribute(geometry, glAttribute);
            }
            
            // Bind vertex array and draw
            glBindVertexArray(array_id);
            glDrawArrays(drawmode, 0, no_vertices);
            
            check_gl_error();
        }
        
        template<class T>
        static void update_attribute(std::shared_ptr<mesh::Mesh> geometry, std::shared_ptr<GLVertexAttribute<T>> attribute)
        {
            if(attribute->is_up_to_date())
                return;
            if(geometry->get_no_faces() > 0)
            {
                for(auto face = geometry->faces_begin(); face != geometry->faces_end(); face = face->next())
                {
                    attribute->add_data_at(*face->v1());
                    attribute->add_data_at(*face->v2());
                    attribute->add_data_at(*face->v3());
                }
            }
            else if(geometry->get_no_edges() > 0)
            {
                for(auto edge = geometry->edges_begin(); edge != geometry->edges_end(); edge = edge->next())
                {
                    attribute->add_data_at(*edge->v1());
                    attribute->add_data_at(*edge->v2());
                }
            }
            else if(geometry->get_no_vertices() > 0)
            {
                for(auto vertex = geometry->vertices_begin(); vertex != geometry->vertices_end(); vertex = vertex->next())
                {
                    attribute->add_data_at(*vertex);
                }
            }
            attribute->send_data();
        }
    };
}
