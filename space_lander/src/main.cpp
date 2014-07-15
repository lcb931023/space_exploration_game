#include <iostream>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Box2D/Box2D.h>

const float FPS = 60;

float shipX = 200, shipY = 200;
float thrustX = 0, thrustY = 0;
float velX = 0, velY = 0;

struct world {
};

struct camera {
    int x, y;
    int w, h;
};

int main (int argc, char** argv) {
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    ALLEGRO_TIMER *timer = nullptr;
    bool done = false;
    bool redraw = true;

    // initialize all allegro systems
    if (!al_init () || !al_init_primitives_addon () || !al_install_keyboard () ) {
        std::cout << "failed to initialize allegro!" << std::endl;
        return -1;
    }

    timer = al_create_timer (1.0 / FPS);
    if (!timer) {
        std::cout << "failed to create timer!" << std::endl;
        return -1;
    }
 
    display = al_create_display (800, 600);
    if (!display) {
        std::cout << "failed to create display!" << std::endl;
        al_destroy_timer (timer);
        return -1;
    }
 
    event_queue = al_create_event_queue ();
    if (!event_queue) {
        std::cout << "failed to create event_queue!" << std::endl;
        al_destroy_display (display);
        al_destroy_timer (timer);
        return -1;
    }

    // register event sources for events to come from
    al_register_event_source (event_queue, al_get_display_event_source (display) );
    al_register_event_source (event_queue, al_get_timer_event_source (timer) );
    al_register_event_source (event_queue, al_get_keyboard_event_source () );

    // clear the screen and start the FPS timer
    al_clear_to_color (al_map_rgb (0, 0, 0) );
    al_flip_display ();
    al_start_timer (timer);

    while (!done) {
        // poll events
        ALLEGRO_EVENT ev;
        al_wait_for_event (event_queue, &ev);
 
        // check for events
        if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    thrustY = -0.02;
                    break;
                case ALLEGRO_KEY_LEFT:
                    thrustX = -0.02;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    thrustX = 0.02;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    thrustY = 0;
                    break;
                case ALLEGRO_KEY_LEFT:
                    thrustX = 0;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    thrustX = 0;
                    break;
            }
        }

        // do logic
        velX += thrustX;
        velY += thrustY;

        if (velY < 1.0)
            velY += 0.01;

        shipX += velX;
        shipY += velY;

        // redraw if there are no more events
        if (redraw && al_is_event_queue_empty (event_queue) ) {
            redraw = false;
            al_clear_to_color (al_map_rgb (0,0,0) );

            al_draw_filled_circle (shipX, shipY, 20, al_map_rgb (255, 255, 255) );

            al_flip_display ();
        }
    }

    // clean up resources
    al_destroy_timer (timer);
    al_destroy_display (display);
    al_destroy_event_queue (event_queue);
    al_shutdown_primitives_addon ();

    return 0;
}
