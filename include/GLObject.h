//
//  Created by Asger Nyman Christiansen on 07/11/15.
//  Copyright © 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include "Mesh.h"
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
        std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>> vec2_vertex_attributes;
        std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>> vec3_vertex_attributes;
        
        template<class T>
        void update_attribute(std::shared_ptr<GLVertexAttribute<T>> attribute) const
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
        
    public:
        
        GLObject(std::shared_ptr<mesh::Mesh> geometry, std::shared_ptr<GLMaterial> material) : material(material), geometry(geometry)
        {
            // Generate and bind array
            glGenVertexArrays(1, &array_id);
            glBindVertexArray(array_id);
            
            vec3_vertex_attributes.push_back(material->create_attribute("position", geometry->position()));
            material->create_attributes(vec2_vertex_attributes, vec3_vertex_attributes);
        }
        
        /**
         Draws the object.
         */
        void draw(const glm::vec3& camera_position, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
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
            for (auto glAttribute : vec2_vertex_attributes)
            {
                update_attribute(glAttribute);
            }
            
            for (auto glAttribute : vec3_vertex_attributes)
            {
                update_attribute(glAttribute);
            }
            
            // Use material specific uniforms and states
            material->pre_draw(camera_position, model, view, projection);
            
            // Bind vertex array and draw
            glBindVertexArray(array_id);
            glDrawArrays(drawmode, 0, no_vertices);
            
            check_gl_error();
        }
    };
}
