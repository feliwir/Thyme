add_executable(w3drender)
target_sources(w3drender PRIVATE
    w3drender.cpp
)
target_link_libraries(w3drender PRIVATE thyme_lib cxxopts::cxxopts SDL2::SDL2main)