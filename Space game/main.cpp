#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <FractalNoise.h>
#include <stdlib.h>     /* abs */

const float FPS = 60;
FractalNoise p;
float zPerlin = 0.7;

float fClamp (float pf, float min, float max) {
	if (pf >= max) pf = max;
	if (pf <= min) pf = min;
	return pf;
}

void drawStars () {
	float x;
	float y;
	float r;
	p.setPersistence(1);
	for (int i=0; i<30; i++)
	{
		x =	400 + 400 * p.noise(i * 0.03, i * 0.05, (i+1) * 0.02);
		y =	300 + 300 * p.noise(i * 0.05, i * 0.03, (i+1) * 0.06);
		r = 30 + 20 * p.noise(i * 0.036, i * 0.063, (i+1) * 0.086);
		al_draw_filled_circle (x, y, r, al_map_rgb (255, 255, 255) );
	}
}
/*** Star texture Charts 

Sun's inner texture: setPersistence(0.6), noise(x * 0.2,y * 0.2, z)
Moon & Mercury: setPersistence(1), noise(x * 0.007,y * 0.008, z)
Jupiter: setPersistence(0.1), noise(x * 0.002,y * 0.02, z)
Venus: setPersistence(0.6), noise(x * 0.2,y * 0.2, z)

***/
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
	drawStars();
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
            //al_clear_to_color (al_map_rgb (0,0,0) );
            //drawStars();
			//zPerlin += 0.1;
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