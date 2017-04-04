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
#include "GLAmbientOcclusionEffect.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"

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

void update(GLCamera& camera)
{
    static float last_time = time();
    float current_time = time();
    float elapsed_time = current_time - last_time;
    last_time = current_time;
    
    print_fps(elapsed_time);
    *cube_rotation_angle = current_time;
    
    static vec3 view_position = vec3(0., 0., 5.);
    static vec3 view_direction = vec3(0., 0., -1.);
    const float speed = 3.;
    
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    
    if(currentKeyStates[ SDL_SCANCODE_S ])
    {
        view_position -= speed * elapsed_time * view_direction;
    }
    else if(currentKeyStates[ SDL_SCANCODE_W ])
    {
        view_position += speed * elapsed_time * view_direction;
    }
    
    auto side = normalize(cross(view_direction, vec3(0.,1.,0.)));
    auto up = normalize(cross(side, view_direction));
    if(currentKeyStates[ SDL_SCANCODE_A ])
    {
        view_direction = vec3(glm::rotate(mat4(), elapsed_time, up) * vec4(view_direction, 1.));
    }
    else if(currentKeyStates[ SDL_SCANCODE_D ])
    {
        view_direction = vec3(glm::rotate(mat4(), -elapsed_time, up) * vec4(view_direction, 1.));
    }
    else if(currentKeyStates[ SDL_SCANCODE_E ])
    {
        view_direction = vec3(glm::rotate(mat4(), -elapsed_time, side) * vec4(view_direction, 1.));
    }
    else if(currentKeyStates[ SDL_SCANCODE_Q ])
    {
        view_direction = vec3(glm::rotate(mat4(), elapsed_time, side) * vec4(view_direction, 1.));
    }
    camera.set_view(view_position, view_direction);
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
    
    // Create scene
    auto scene = GLScene();
    create_room(scene);
    create_cube(scene);
    scene.add_light(std::make_shared<GLPointLight>(glm::vec3(-1., 5., 1.)));
    scene.add_light(std::make_shared<GLDirectionalLight>(glm::vec3(1., -1., 0.)));
    
    // Create post effect
    auto ssao = GLAmbientOcclusionEffect();
    
    // run while the window is open
    bool quit = false;
    while(!quit)
    {
        // process pending events
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }
        
        // update the scene based on the time elapsed since last update
        update(camera);
        
        // draw one frame
        camera.draw(scene);
        camera.apply_post_effect(ssao);
        
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
