//
//  main.cpp
//  GLEngineExample1
//
//  Created by Asger Nyman Christiansen on 18/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#include "GLCamera.h"
#include "MeshCreator.h"
#include "materials/GLColorMaterial.h"
#include "gtx/rotate_vector.hpp"

#define GLFW_INCLUDE_NONE
#include "glfw3.h"

using namespace std;
using namespace glm;
using namespace gle;
using namespace mesh;

GLFWwindow* gWindow = NULL;

shared_ptr<float> cube_rotation_angle = make_shared<float>(0.f);

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

void update(GLCamera& camera)
{
    static float last_time = glfwGetTime();
    float time = glfwGetTime();
    float elapsed_time = time - last_time;
    
    print_fps(elapsed_time);
    *cube_rotation_angle = time;
    
    static vec3 view_position = vec3(0., 0., 5.);
    static vec3 view_direction = vec3(0., 0., -1.);
    const float speed = 3.;
    
    if(glfwGetKey(gWindow, 'S'))
    {
        view_position -= speed * elapsed_time * view_direction;
    }
    else if(glfwGetKey(gWindow, 'W'))
    {
        view_position += speed * elapsed_time * view_direction;
    }
    
    auto side = normalize(cross(view_direction, vec3(0.,1.,0.)));
    auto up = normalize(cross(side, view_direction));
    if(glfwGetKey(gWindow, 'A'))
    {
        view_direction = vec3(glm::rotate(mat4(), elapsed_time, up) * vec4(view_direction, 1.));
    }
    else if(glfwGetKey(gWindow, 'D'))
    {
        view_direction = vec3(glm::rotate(mat4(), -elapsed_time, up) * vec4(view_direction, 1.));
    }
    else if(glfwGetKey(gWindow, 'E'))
    {
        view_direction = vec3(glm::rotate(mat4(), -elapsed_time, side) * vec4(view_direction, 1.));
    }
    else if(glfwGetKey(gWindow, 'Q'))
    {
        view_direction = vec3(glm::rotate(mat4(), elapsed_time, side) * vec4(view_direction, 1.));
    }
    camera.set_view(view_position, view_direction);
    
    last_time = time;
}

void create_cube(GLScene& root)
{
    auto material = make_shared<GLColorMaterial>(vec3(0.5, 0.1, 0.7));
    auto geometry = MeshCreator::create_box(false);
    
    auto rotation_node = std::make_shared<GLRotationNode>(vec3(1., 1., 0.), cube_rotation_angle);
    
    root.add_child(rotation_node)->add_leaf(geometry, material);
}

void create_room(GLScene& root)
{
    auto color_material = make_shared<GLColorMaterial>(vec3(0.5, 0.5, 0.5));
    auto box = MeshCreator::create_box(true);
    auto scale_node = std::make_shared<GLTransformationNode>(scale(vec3(10., 10., 10.)));
    root.add_child(scale_node)->add_leaf(box, color_material);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow(WIN_SIZE_X, WIN_SIZE_Y, "GLEngine example 1", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
    
    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwMakeContextCurrent(gWindow);
    glfwGetFramebufferSize(gWindow, &WIN_SIZE_X, &WIN_SIZE_Y);
    
    // Create camera
    auto camera = GLCamera(WIN_SIZE_X, WIN_SIZE_Y);
    
    // Create scene
    auto scene = GLScene();
    create_room(scene);
    create_cube(scene);
    scene.add_light(std::make_shared<GLPointLight>(glm::vec3(-1., 5., 1.)));
    scene.add_light(std::make_shared<GLDirectionalLight>(glm::vec3(1., -1., 0.)));
    
    // run while the window is open
    while(!glfwWindowShouldClose(gWindow))
    {
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        update(camera);
        
        // draw one frame
        camera.draw(scene);
        
        glfwSwapBuffers(gWindow);
        
        //exit program if escape key is pressed
        if(glfwGetKey(gWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(gWindow, GL_TRUE);
    }
    
    // clean up and exit
    glfwTerminate();
    return 0;
}
