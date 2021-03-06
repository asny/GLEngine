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
        
        bool should_draw_when(DrawPassMode mode) const
        {
            return material->should_draw(mode);
        }
        
        void draw(const DrawPassInput& input, const glm::mat4& model)
        {
            // Infer draw mode and number of vertices
            auto drawmode = get_drawmode();
            auto no_vertices = get_number_of_vertices();
            if(no_vertices == 0)
                return;
            
            // Update buffers if needed
            if(should_update_buffers)
            {
                update_buffers();
            }
            
            // Use material specific uniforms and states
            material->pre_draw(input, model);
            
            // Bind vertex array and draw
            glBindVertexArray(array_id);
            glDrawArrays(drawmode, 0, no_vertices);
            
            check_gl_error();
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
                auto position = GLVertexAttribute<glm::vec3>::use(*shader, "position", mesh->position());
                update_attribute(mesh, position);
                position->send_data();
                
                auto uv = GLVertexAttribute<glm::vec2>::use(*shader, "uv_coordinates", uv_coordinates);
                update_attribute(mesh, uv);
                uv->send_data();
            }
            glBindVertexArray(array_id);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        void invalidate(std::shared_ptr<mesh::Mesh> mesh)
        {
            if(mesh != geometry)
                return;
            
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
            should_update_buffers = true;
        }
        
    private:
        
        void update_buffers()
        {
            for (auto glAttribute : float_vertex_attributes)
            {
                glAttribute->send_data();
            }
            
            for (auto glAttribute : vec2_vertex_attributes)
            {
                glAttribute->send_data();
            }
            
            for (auto glAttribute : vec3_vertex_attributes)
            {
                glAttribute->send_data();
            }
            should_update_buffers = false;
        }
        
        GLenum get_drawmode()
        {
            if(geometry->get_no_faces() > 0)
            {
                return GL_TRIANGLES;
            }
            if(geometry->get_no_edges() > 0)
            {
                return GL_LINES;
            }
            return GL_POINTS;
        }
        
        unsigned int get_number_of_vertices()
        {
            if(geometry->get_no_faces() > 0)
            {
                return geometry->get_no_faces() * 3;
            }
            else if(geometry->get_no_edges() > 0)
            {
                return geometry->get_no_edges() * 2;
            }
            else if(geometry->get_no_vertices() > 0)
            {
                return geometry->get_no_vertices();
            }
            return 0;
        }
        
        template<class T>
        static void update_attribute(std::shared_ptr<mesh::Mesh> geometry, std::shared_ptr<GLVertexAttribute<T>> attribute)
        {
            attribute->clear();
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
        }
        
        const std::shared_ptr<mesh::Mesh> geometry;
        const std::shared_ptr<GLMaterial> material;
        
        GLuint array_id;
        std::vector<std::shared_ptr<GLVertexAttribute<float>>> float_vertex_attributes;
        std::vector<std::shared_ptr<GLVertexAttribute<glm::vec2>>> vec2_vertex_attributes;
        std::vector<std::shared_ptr<GLVertexAttribute<glm::vec3>>> vec3_vertex_attributes;
        bool should_update_buffers = false;
    };
}
