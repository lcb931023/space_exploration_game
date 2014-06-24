cmake_minimum_required (VERSION 2.8)
project (SPACE_GAME)

ADD_DEFINITIONS(
    -std=c++11
)

include_directories ("${SPACE_GAME_SOURCE_DIR}/thirdparty/include"
    "${SPACE_GAME_SOURCE_DIR}/include")

link_directories ("/usr/local/lib")

add_executable (space_game
    src/main.cpp
    src/game_context.cpp
    src/world.cpp)

target_link_libraries (space_game
    allegro
    allegro_main
    allegro_primitives
    "${SPACE_GAME_SOURCE_DIR}/thirdparty/lib/perlin.a")
