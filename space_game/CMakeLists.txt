cmake_minimum_required (VERSION 2.8)
project (space_game)

link_directories (/usr/local/lib)

add_executable (space_game
    src/main.cpp)

target_link_libraries (space_game allegro allegro_main allegro_primitives)
