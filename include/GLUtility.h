//
//  GLUtility.h
//  Spider
//
//  Created by Asger Nyman Christiansen on 07/11/15.
//  Copyright © 2015 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

#include <OpenGL/gl3.h>

#include <iostream>
#include <vector>
#include <map>

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat4x4.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_inverse.hpp"

namespace gle {
    
    const static unsigned int NULL_LOCATION = -1;
    
    inline void _check_gl_error(const char *file, int line)
    {
        GLenum err (glGetError());
        
        while(err!=GL_NO_ERROR) {
            std::string error;
            
            switch(err) {
                case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
                case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
                case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
                case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
            }
            
            std::cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<std::endl;
            err=glGetError();
        }
    }
    
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)
    
    static float time()
    {
        static auto start_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        return 0.001f * std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
    }
    
    static double random(double min, double max)
    {
        return (max - min) * (double)rand()/(double)RAND_MAX + min;
    }
}
