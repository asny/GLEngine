//
//  Created by Asger Nyman Christiansen on 18/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#include "GLCamera.h"
#include "MeshCreator.h"
#include "materials/GLFlatColorMaterial.h"
#include "materials/GLColorMaterial.h"

#define GLFW_INCLUDE_NONE
#include "glfw3.h"

using namespace std;
using namespace glm;
using namespace gle;
using namespace mesh;

GLFWwindow* gWindow = NULL;

shared_ptr<mat4> cube_rotation = make_shared<mat4>(1.);

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
    *cube_rotation = rotate(rotate(*cube_rotation, elapsed_time, vec3(0., 1., 0.)), elapsed_time, vec3(1., 0., 0.));
    
    last_time = time;
}

void create_cube(GLScene& root, const vec3& translation, std::shared_ptr<GLMaterial> material)
{
    auto geometry = MeshCreator::create_box(false);
    
    auto translation_node = std::make_shared<GLTransformationNode>(translate(translation));
    root.add_child(translation_node);
    auto rotation_node = std::make_shared<GLTransformationNode>(cube_rotation);
    translation_node->add_child(rotation_node);
    rotation_node->add_leaf(geometry, material);
}

void create_cubes(GLScene& root)
{
    auto flat_material = make_shared<GLFlatColorMaterial>(vec3(0.5, 0.1, 0.7));
    auto color_material = make_shared<GLColorMaterial>(vec3(0.5, 0.1, 0.7));
    
    create_cube(root, vec3(-2., 2., 0.), color_material);
    create_cube(root, vec3(-2., -2., 0.), flat_material);
    create_cube(root, vec3(2., 2., 0.), flat_material);
    create_cube(root, vec3(2., -2., 0.), color_material);
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
    camera.set_view(vec3(0., 0., 10.), vec3(0., 0., -1.));
    
    // Create scene
    auto scene = GLScene();
    scene.add_light(std::make_shared<GLDirectionalLight>(glm::vec3(1., -1., 0.)));
    create_cubes(scene);
    
    // run while the window is open
    while(!glfwWindowShouldClose(gWindow))
    {
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        update();
        
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
