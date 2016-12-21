//
//  main.cpp
//  GLEngineExample1
//
//  Created by Asger Nyman Christiansen on 18/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#include "GLScene.h"
#include "MeshCreator.h"
#include "materials/GLFlatMaterial.h"
#include "gtx/rotate_vector.hpp"

#define GLFW_INCLUDE_NONE
#include "glfw3.h"

using namespace std;
using namespace glm;
using namespace oogl;
using namespace geogo;

GLFWwindow* gWindow = NULL;

shared_ptr<mat4> cube1_rotation = make_shared<mat4>(1.);
shared_ptr<mat4> cube2_rotation = make_shared<mat4>(1.);

void on_error(int errorCode, const char* msg)
{
    throw std::runtime_error(msg);
}

void print_fps(double elapsedTime)
{
    static int draws = 0;
    draws++;
    static float seconds = 0.;
    seconds += elapsedTime;
    if(seconds > 5)
    {
        std::cout << "FPS: " << ((float)draws)/seconds << std::endl;
        seconds = 0.;
        draws = 0;
    }
}

void update()
{
    static float last_time = glfwGetTime();
    float time = glfwGetTime();
    float elapsed_time = time - last_time;
    
    print_fps(elapsed_time);
    *cube1_rotation = rotate(rotate(*cube1_rotation, elapsed_time, vec3(0., 1., 0.)), elapsed_time, vec3(1., 0., 0.));
    *cube2_rotation = rotate(rotate(*cube2_rotation, elapsed_time, vec3(0., 1., 0.)), elapsed_time, vec3(1., 0., 0.));
    
    last_time = time;
}

void create_cubes(GLScene& scene)
{
    {
        auto geometry = MeshCreator::create_box(false);
        auto material = shared_ptr<GLMaterial>(new GLFlatMaterial({0.5, 0.5, 0.5}, {0., 0.5, 0.}, {0.5, 0., 0.}, 1.));
        auto object = make_shared<GLObject>(geometry, material);
        *cube1_rotation = translate(vec3(-1., 0., 1.));
        object->set_model_matrix(cube1_rotation);
        scene.add(object);
    }
    {
        auto geometry = MeshCreator::create_box(false);
        auto material = shared_ptr<GLMaterial>(new GLFlatMaterial({0.5, 0.5, 0.5}, {0., 0.5, 0.}, {0.5, 0., 0.}, 1.));
        auto object = make_shared<GLObject>(geometry, material);
        *cube2_rotation = translate(vec3(1., 0., -1.));
        object->set_model_matrix(cube2_rotation);
        scene.add(object);
    }
}

int main(int argc, const char * argv[])
{
    int WIN_SIZE_X = 1200;
    int WIN_SIZE_Y = 700;
    
    // initialise GLFW
    glfwSetErrorCallback(on_error);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // Open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow(WIN_SIZE_X, WIN_SIZE_Y, "GLEngine example 1", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwMakeContextCurrent(gWindow);
    glfwGetFramebufferSize(gWindow, &WIN_SIZE_X, &WIN_SIZE_Y);
    
    // Create camera
    auto camera = make_shared<GLCamera>();
    camera->set_screen_size(WIN_SIZE_X, WIN_SIZE_Y);
    camera->set_view(vec3(5.,0.,5.), vec3(-1., 0., -1.));
    
    // Create scene
    auto scene = unique_ptr<GLScene>(new GLScene(camera));
    
    // Create object
    create_cubes(*scene);
    
    // run while the window is open
    while(!glfwWindowShouldClose(gWindow))
    {
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        update();
        
        // draw one frame
        scene->draw();
        
        glfwSwapBuffers(gWindow);
        
        //exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }
    
    // clean up and exit
    glfwTerminate();
    return 0;
}
