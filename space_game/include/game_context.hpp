#ifndef GAME_CONTEXT_HPP
#define GAME_CONTEXT_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <world.hpp>
#include <camera.hpp>

namespace space {
    class game_context {
    public:
        game_context (ALLEGRO_DISPLAY* _display, ALLEGRO_EVENT_QUEUE* _evqueue);
        ~game_context ();

        ALLEGRO_DISPLAY *display;
        ALLEGRO_EVENT_QUEUE *event_queue;

        world w;
        camera c;
    };
}

#endif
