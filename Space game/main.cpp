#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Perlin.h>
#include <stdlib.h>     /* abs */

const float FPS = 60;
Perlin p;
float zPerlin = 0.01;

void drawStars () {
	for (int i = 0; i < 600/4; i++)
	{
		for (int j = 0; j < 800/4; j++)
		{
			// perlin noise ENTIRE SCREEN
			float n = abs(p.noise(i * 0.3, j * 0.3, zPerlin) * 255);
			//std::cout << n << std::endl;
			al_draw_filled_circle (j * 4 + 2, i * 4 + 2, 1, al_map_rgb (n,n,n) );
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
            al_clear_to_color (al_map_rgb (0,0,0) );
            drawStars();
			zPerlin += 0.1;
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