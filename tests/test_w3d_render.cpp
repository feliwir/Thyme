/**
 * @file
 *
 * @author feliwir
 *
 * @brief Set of tests to validate the W3D rendering implementation
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include <gtest/gtest.h>
#include <w3d.h>
#ifdef BUILD_WITH_SDL2
#include <SDL.h>
#endif

TEST(w3d_render, init)
{
#ifdef BUILD_WITH_SDL2
    ASSERT_GE(SDL_Init(SDL_INIT_VIDEO), 0);
    SDL_GL_SetSwapInterval(1);
    SDL_Window *app_window = SDL_CreateWindow("w3d_render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, 0);
#endif
    EXPECT_EQ(W3D::Init(nullptr, nullptr, false), W3D_ERROR_OK);

    EXPECT_EQ(W3D::Set_Render_Device(0, 400, 300, 32, 1, true, false, true), W3D_ERROR_OK);

    int frames = 1000;
    while (frames > 0) {
#ifdef BUILD_WITH_SDL2
        SDL_Event event;
        while (SDL_PollEvent(&event)) { // poll until all events are handled!
            switch (event.type) {
                case SDL_QUIT:
                    frames = 0;
                    break;

                default:
                    break;
            }
        }
#endif

        EXPECT_EQ(W3D::Begin_Render(true), W3D_ERROR_OK);

        EXPECT_EQ(W3D::End_Render(true), W3D_ERROR_OK);
#ifdef BUILD_WITH_SDL2
        SDL_GL_SwapWindow(app_window);
#endif
        frames--;
    }

    W3D::Shutdown();
#ifdef BUILD_WITH_SDL2
    SDL_DestroyWindow(app_window);
    SDL_Quit();
#endif
}
