add_library(flextGL STATIC flextGL.c)
target_include_directories(flextGL PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(OpenGL REQUIRED)
target_link_libraries(flextGL PUBLIC OpenGL::GL)