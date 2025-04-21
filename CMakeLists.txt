cmake_minimum_required(VERSION 3.16)
project(game44)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML path (update if installed elsewhere)
set(SFML_ROOT "D:/SFML-2.6.2/SFML-2.6.2")

# Enable debug suffix for libraries like sfml-graphics-d etc.
set(SFML_STATIC_LIBRARIES FALSE)
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

# Find SFML with required components
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Add your source file
add_executable(game44 main.cpp)

# Link SFML libraries including audio
target_link_libraries(game44
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)
