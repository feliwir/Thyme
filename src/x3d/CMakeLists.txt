option(X3D_ENABLE_OPENGL "Use OpenGL" ON)
option(X3D_ENABLE_D3D9 "Use D3D9" ON)

add_library(x3d x3d.cpp x3d_context.cpp x3d_texture.cpp)

if(X3D_ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)

    if(OPENGL_FOUND)
        target_sources(x3d PRIVATE
            gl/x3d_buffer_gl.cpp
            gl/x3d_context_gl.cpp
            gl/x3d_shader_gl.cpp
            gl/x3d_texture_gl.cpp
            gl/x3d_vertexlayout_gl.cpp)
        target_compile_definitions(x3d PRIVATE X3D_HAS_OPENGL)
        target_include_directories(x3d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gl)
        target_link_libraries(x3d PRIVATE OpenGL::GL)
    endif()

    set(XSC_BUILD_SHELL OFF CACHE INTERNAL "Turn off shell")
    # We need XShaderCompiler
    FetchContent_Declare(
        xsc
        GIT_REPOSITORY https://github.com/LukasBanana/XShaderCompiler.git
        GIT_TAG        c1d5018f80beaaf7eca8f6b1ab6952b3f2841be5
    )
    FetchContent_MakeAvailable(xsc)

    target_include_directories(x3d PRIVATE ${xsc_SOURCE_DIR}/inc)

    # We added this subdirectory in the toplevel CMakeLists.txt
    target_link_libraries(x3d PRIVATE flextGL xsc_core)
endif()

if(X3D_ENABLE_D3D9 AND WIN32)
    target_sources(x3d PRIVATE 
        d3d9/x3d_buffer_d3d9.cpp
        d3d9/x3d_context_d3d9.cpp)
    target_compile_definitions(x3d PRIVATE X3D_HAS_D3D9)
endif()

target_include_directories(x3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(x3d PUBLIC BUILD_WITH_X3D)