cmake_minimum_required(VERSION 3.10)
project(FlappyBird)

# Find SFML
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Add your main file
add_executable(FlappyBird main.cpp)

# Link SFML to your game
target_link_libraries(FlappyBird SFML::Graphics SFML::Window SFML::System)