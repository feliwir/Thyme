option(X3D_ENABLE_OPENGL "Use OpenGL" ON)
option(X3D_ENABLE_D3D9 "Use D3D9" ON)

add_library(x3d x3d.cpp x3d_context.cpp)

if(X3D_ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)

    if(OPENGL_FOUND)
        target_sources(x3d PRIVATE
            gl/x3d_buffer_gl.cpp
            gl/x3d_context_gl.cpp
            gl/x3d_shader_gl.cpp
            gl/x3d_vertexlayout_gl.cpp)
        target_compile_definitions(x3d PRIVATE X3D_HAS_OPENGL)
        target_include_directories(x3d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gl)
        target_link_libraries(x3d PRIVATE OpenGL::GL)
    endif()

    # We need HLSL2GLSL
    FetchContent_Declare(
        hlsl2glsl
        GIT_REPOSITORY https://github.com/feliwir/hlsl2glslfork.git
        GIT_TAG        acaf94c6324ed7279df6aa5b7bda246da8a779d6
    )
    FetchContent_MakeAvailable(hlsl2glsl)

    # We added this subdirectory in the toplevel CMakeLists.txt
    target_link_libraries(x3d PRIVATE flextGL hlsl2glsl)
endif()

if(X3D_ENABLE_D3D9 AND WIN32)
    target_sources(x3d PRIVATE 
        d3d9/x3d_buffer_d3d9.cpp
        d3d9/x3d_context_d3d9.cpp)
    target_compile_definitions(x3d PRIVATE X3D_HAS_D3D9)
endif()

target_include_directories(x3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(x3d PUBLIC BUILD_WITH_X3D)