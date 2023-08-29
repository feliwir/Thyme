add_executable(imagepacker)
target_sources(imagepacker PRIVATE
    imagepacker.cpp
)
target_link_libraries(imagepacker PRIVATE thyme_lib cxxopts::cxxopts)
