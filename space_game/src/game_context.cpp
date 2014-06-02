#include <game_context.hpp>
#include <world.hpp>
#include <camera.hpp>

namespace space {
    game_context::game_context (ALLEGRO_DISPLAY* _display, ALLEGRO_EVENT_QUEUE* _evqueue) {
        display = _display;
        event_queue = _evqueue;
    }

    game_context::~game_context () {
    }

    void game_context::update () {
    }

    void game_context::render () {
        al_draw_circle (320, 240, 100, al_map_rgb (255, 255, 255), 2);
    }
}