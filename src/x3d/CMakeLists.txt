add_library(x3d x3d.cpp)
target_include_directories(x3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(x3d PUBLIC BUILD_WITH_X3D)