cmake_minimum_required(VERSION 3.16)
project(mastermind LANGUAGES C)

find_package(SDL3 REQUIRED)
find_package(SDL3_TTF REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(mastermind src/main.c src/gui.c src/script.c)
target_link_libraries(mastermind PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)
