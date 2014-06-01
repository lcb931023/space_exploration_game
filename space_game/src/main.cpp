#include <iostream>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <perlin/Perlin.h>

const float FPS = 60;
Perlin p;
float perlin_z = 0;

void draw_stars () {
    for (int x = 0; x < 640; x += 2) {
        for (int y = 0; y < 480; y += 2) {
            float v = p.noise (x * 0.5, y * 0.5, perlin_z * 0.2);
            al_draw_filled_circle (x, y, v * 6, al_map_rgb (x * v, y * v, abs (x - y) * v) );
        }
    }
}

int main (int argc, char** argv) {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool done = false;
    bool redraw = true;

    // initialize all allegro systems
    if (!al_init () || !al_init_primitives_addon () ) {
        std::cout << "failed to initialize allegro!" << std::endl;
        return -1;
    }

    timer = al_create_timer (1.0 / FPS);
    if (!timer) {
        std::cout << "failed to create timer!" << std::endl;
        return -1;
    }
 
    display = al_create_display (640, 480);
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

    // clear the screen and start the FPS timer
    al_clear_to_color (al_map_rgb (0,0,0) );
    al_flip_display ();
    al_start_timer (timer);

    while (!done)
    {
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

        // redraw if there are no more events
        if (redraw && al_is_event_queue_empty (event_queue) ) {
            redraw = false;
            draw_stars ();
            al_flip_display ();

            perlin_z += 0.5;
        }
    }

    // clean up resources
    al_destroy_timer (timer);
    al_destroy_display (display);
    al_destroy_event_queue (event_queue);
    al_shutdown_primitives_addon ();

    return 0;
}
