/**
 * @file
 *
 * @author feliwir
 *
 * @brief A lightweight W3D renderer
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include <assetmgr.h>
#include <camera.h>
#include <scene.h>
#include <w3d.h>

#include <SDL.h>
#include <cxxopts.hpp>

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;

#define W3D_OK(S) \
    if ((S) != W3D_ERROR_OK) \
        return EXIT_FAILURE;

int main(int argc, char **argv)
{
    cxxopts::Options options("W3DRender", "A lightweight W3D renderer");

    options.allow_unrecognised_options();

    // clang-format off
    options.add_options()
    ("i,input", "input file", cxxopts::value<std::string>())
    ("h,help", "print usage")
    ("v,verbose", "verbose output", cxxopts::value<bool>()->default_value("false"))
    ;
    // clang-format on

    const auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (result.count("input") != 1) {
        std::cerr << "Please specify exactly one input" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    captains_settings_t captains_settings = { 0 };
    captains_settings.level = result.count("verbose") > 0 ? LOGLEVEL_TRACE : LOGLEVEL_DEBUG;
    captains_settings.console = true;
    captains_settings.print_file = true;
    captainslog_init(&captains_settings);

    SimpleSceneClass *scene = new SimpleSceneClass();
    CameraClass *camera = new CameraClass();
    W3DAssetManager *asset_mgr = new W3DAssetManager();
    const auto input_path = result["input"].as<std::string>();
    if (!asset_mgr->Load_3D_Assets(input_path.c_str())) {
        std::cerr << "Failed to load W3D assert" << std::endl;
        return EXIT_FAILURE;
    }

    RenderObjClass *robj = asset_mgr->Create_Render_Obj("cube");
    scene->Add_Render_Object(robj);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL2" << std::endl;
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    SDL_Window *app_window = SDL_CreateWindow("Westwood 3D (W3D) - Render",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DEFAULT_WIDTH,
        DEFAULT_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (app_window == nullptr) {
        std::cerr << "Failed to create a window" << std::endl;
        return EXIT_FAILURE;
    }

    SDL_GLContext gl_ctx = SDL_GL_CreateContext(app_window);
    SDL_RaiseWindow(app_window);

    W3D_OK(W3D::Init(nullptr, nullptr, false));
    W3D_OK(W3D::Set_Render_Device(0, DEFAULT_WIDTH, DEFAULT_HEIGHT, 32, 1, true, false, true));

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // poll until all events are handled!
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                default:
                    break;
            }
        }

        W3D_OK(W3D::Begin_Render(true, true, Vector3(0.2, 0.2, 0.2), 1.0f));
        W3D_OK(W3D::Render(scene, camera));
        W3D_OK(W3D::End_Render(true));
        SDL_GL_SwapWindow(app_window);
    }

    W3D::Shutdown();
    delete camera;
    delete scene;

    SDL_GL_DeleteContext(gl_ctx);
    SDL_DestroyWindow(app_window);
    SDL_Quit();
    return EXIT_SUCCESS;
}