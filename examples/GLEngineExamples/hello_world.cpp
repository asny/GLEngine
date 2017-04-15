//
//  Created by Asger Nyman Christiansen on 18/12/2016.
//  Copyright © 2016 Asger Nyman Christiansen. All rights reserved.
//

#include "GLCamera.h"
#include "MeshCreator.h"
#include "materials/GLFlatColorMaterial.h"
#include "materials/GLColorMaterial.h"
#include "materials/GLStandardMaterial.h"
#include "effects/GLDebugEffect.h"
#include "gtx/rotate_vector.hpp"

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

void update()
{
    static float last_time = time();
    float current_time = time();
    float elapsed_time = current_time - last_time;
    
    print_fps(elapsed_time);
    *cube_rotation_angle = current_time;
    
    last_time = current_time;
}

void create_cube(GLScene& root, const vec3& translation, std::shared_ptr<GLMaterial> material)
{
    auto geometry = MeshCreator::create_box(false);
    
    auto translation_node = std::make_shared<GLTranslationNode>(translation);
    auto rotation_node = std::make_shared<GLRotationNode>(vec3(1., 1., 0.), cube_rotation_angle);
    
    root.add_child(translation_node)->add_child(rotation_node)->add_leaf(geometry, material);
}

void create_cubes(GLScene& root)
{
    auto flat_material = make_shared<GLFlatColorMaterial>(vec3(0.5, 0.1, 0.7));
    auto color_material = make_shared<GLColorMaterial>(vec3(0.5, 0.1, 0.7));
    auto standard_material = make_shared<GLStandardMaterial>(vec3(0.2, 0.2, 0.2), vec3(0.5, 0.1, 0.7), vec3(0.5, 0.1, 0.7), 1.);
    
    create_cube(root, vec3(-2., 2., 0.), color_material);
    create_cube(root, vec3(-2., -2., 0.), standard_material);
    create_cube(root, vec3(2., 2., 0.), standard_material);
    create_cube(root, vec3(2., -2., 0.), color_material);
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
    auto window = SDL_CreateWindow( "Hello world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        throw std::runtime_error("SDL init failed");
    }
    
    // Create context
    auto glcontext = SDL_GL_CreateContext(window);
    
    // Create camera
    auto camera = GLCamera(window_width, window_height);
    camera.set_view(vec3(0., 0., 10.), vec3(0., 0., -1.));
    
    // Create scene
    auto scene = GLScene();
    auto directional_light = make_shared<GLDirectionalLight>();
    scene.add_light(directional_light);
    directional_light->direction = glm::vec3(1., -1., 0.);
    create_cubes(scene);
    
    // Create debug effect
    auto debug_effect = GLDebugEffect();
    
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
            if( e.key.keysym.sym == SDLK_0)
                debug_effect.type = gle::GLDebugEffect::NONE;
            if( e.key.keysym.sym == SDLK_1)
                debug_effect.type = gle::GLDebugEffect::POSITION;
            if( e.key.keysym.sym == SDLK_2)
                debug_effect.type = gle::GLDebugEffect::NORMAL;
            if( e.key.keysym.sym == SDLK_3)
                debug_effect.type = gle::GLDebugEffect::COLOR;
        }
        
        // update the scene based on the time elapsed since last update
        update();
        
        // draw one frame
        camera.draw(scene);
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
