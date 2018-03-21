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
#include "materials/GLFlatColorMaterial.h"
#include "gtx/rotate_vector.hpp"
#include "GLAmbientOcclusionEffect.h"
#include "GLEventHandler.h"

#define SDL_MAIN_HANDLED

using namespace std;
using namespace glm;
using namespace gle;
using namespace mesh;

shared_ptr<float> cube_rotation_angle = make_shared<float>(0.f);

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
    static float last_time = time();
    float current_time = time();
    float elapsed_time = current_time - last_time;
    last_time = current_time;
    
    print_fps(elapsed_time);
    *cube_rotation_angle = current_time;
}

void create_cube(GLScene& root)
{
    auto material = make_shared<GLFlatColorMaterial>(vec3(0.5, 0.1, 0.7));
    auto geometry = MeshCreator::create_box(false);
    
    auto rotation_node = std::make_shared<GLRotationNode>(vec3(1., 1., 0.), cube_rotation_angle);
    
    root.add_child(rotation_node)->add_leaf(geometry, material);
}

void create_room(GLScene& root)
{
    auto color_material = make_shared<GLFlatColorMaterial>(vec3(0.5, 0.5, 0.5));
    auto box = MeshCreator::create_box(true);
    auto scale_node = std::make_shared<GLTransformationNode>(scale(vec3(7., 7., 7.)));
    root.add_child(scale_node)->add_leaf(box, color_material);
}

int main(int argc, const char * argv[])
{
    // Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        throw std::runtime_error("SDL init failed");
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Create window
    int window_width = 1200;
    int window_height = 700;
    auto window = SDL_CreateWindow( "Shadows", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        throw std::runtime_error("SDL init failed");
    }
    
    // Create context
    auto glcontext = SDL_GL_CreateContext(window);
    
    // Create camera
    auto camera = GLCamera(window_width, window_height);
    camera.set_view(vec3(10., 0., 0.), vec3(-1., 0., 0.));
    
    // Create scene
    auto scene = GLScene();
    create_room(scene);
    create_cube(scene);
    
    // Light
    auto material = make_shared<GLColorMaterial>(vec3(1., 1., 1.));
    auto geometry = MeshCreator::create_box(false);
    auto light_translation = make_shared<vec3>(0., 0., 0.);
    scene.add_child(std::make_shared<GLTranslationNode>(light_translation))->add_child(make_shared<GLTransformationNode>(scale(vec3(0.1, 0.1, 0.1))))->add_leaf(geometry, material);
    
    auto point_light = make_shared<GLPointLight>();
    scene.add_light(point_light);
    auto directional_light = make_shared<GLDirectionalLight>();
    scene.add_light(directional_light);
    directional_light->direction = glm::vec3(1., -1., 0.);
    
    // Create post effect
    auto ssao = GLAmbientOcclusionEffect();
    auto debug_effect = GLDebugEffect();
    
    // run while the window is open
    bool quit = false;
    while(!quit)
    {
        // process pending events
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 )
        {
            quit = GLEventHandler::is_quitting(e);
            GLEventHandler::navigate_spherical(e, camera);
            GLEventHandler::show_debug(e, debug_effect);
        }
        
        // update the scene based on the time elapsed since last update
        update();
        point_light->position = vec3(3. * sin(gle::time()), 5. * cos(gle::time()), 0.);
        *light_translation = point_light->position * 1.1f;
        
        // draw one frame
        camera.draw(scene);
        camera.apply_post_effect(ssao);
        camera.apply_post_effect(debug_effect);
        
        SDL_GL_SwapWindow(window);
    }
    
    // Delete context
    SDL_GL_DeleteContext(glcontext);
    
    // Destroy window
    SDL_DestroyWindow( window );
    window = NULL;
    
    // Quit SDL subsystems
    SDL_Quit();
    return 0;
}
