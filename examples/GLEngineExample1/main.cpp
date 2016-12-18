//
//  main.cpp
//  GLEngineExample1
//
//  Created by Asger Nyman Christiansen on 18/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#include "GLScene.h"
#include "MeshCreator.h"
#include "gtx/rotate_vector.hpp"

#define GLFW_INCLUDE_NONE
#include "glfw3.h"

using namespace std;
using namespace glm;
using namespace oogl;
using namespace geogo;

GLFWwindow* gWindow = NULL;

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
    gWindow = glfwCreateWindow(WIN_SIZE_X, WIN_SIZE_Y, "Spider game", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, 0, 0);
    glfwMakeContextCurrent(gWindow);
    
    glfwGetFramebufferSize(gWindow, &WIN_SIZE_X, &WIN_SIZE_Y);
    
    // Create camera
    auto camera = std::shared_ptr<GLCamera>(new GLCamera());
    camera->set_screen_size(WIN_SIZE_X, WIN_SIZE_Y);
    
    // Create scene
    auto scene = unique_ptr<GLScene>(new GLScene(camera));
    
    // Create object
    
    
    // Update
    
    // run while the window is open
    double lastTime = glfwGetTime();
    while(!glfwWindowShouldClose(gWindow))
    {
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        
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
