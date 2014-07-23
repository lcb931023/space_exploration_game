#include <iostream>
#include <vector>
#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Box2D/Box2D.h>

const float FPS = 60;

struct entity {
    float x, y;
    float angle;

    entity (float _x, float _y, float _a) : x (_x), y (_y), angle (_a) {}
};

struct world {
    std::vector<entity> ents;
};

struct camera {
    int x, y;
    int w, h;

    camera (int _x, int _y, int _w, int _h) : x (_x), y (_y), w (_w), h (_h) {}
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

    // init some crap for the game
    float thrustX = 0, thrustY = 0, thrustR = 0;
    float velX = 0, velY = 0, velR = 0;

    entity thing1 (25, 50, 0);
    entity thing2 (80, 80, 0);
    entity thing3 (450, 400, 0);
    entity ship (300, 300, 0);
    world theWorld;
    theWorld.ents.push_back (thing1);
    theWorld.ents.push_back (thing2);
    theWorld.ents.push_back (thing3);

    camera cam (0, 0, 800, 600);

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
                case ALLEGRO_KEY_W:
                    thrustY = -0.01;
                    break;
                case ALLEGRO_KEY_S:
                    thrustY = 0.01;
                    break;
                case ALLEGRO_KEY_A:
                    thrustX = -0.01;
                    break;
                case ALLEGRO_KEY_D:
                    thrustX = 0.01;
                    break;
                case ALLEGRO_KEY_Q:
                    thrustR = 0.00002;
                    break;
                case ALLEGRO_KEY_E:
                    thrustR = -0.00002;
                    break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    thrustY = 0;
                    break;
                case ALLEGRO_KEY_S:
                    thrustY = 0;
                    break;
                case ALLEGRO_KEY_A:
                    thrustX = 0;
                    break;
                case ALLEGRO_KEY_D:
                    thrustX = 0;
                    break;
                case ALLEGRO_KEY_Q:
                    thrustR = 0;
                    break;
                case ALLEGRO_KEY_E:
                    thrustR = 0;
                    break;
            }
        }

        // do logic for moving the ship
        velX += thrustX;
        velY += thrustY;
        velR += thrustR;

        ship.x += velX;
        ship.y += velY;
        ship.angle += velR;

        // redraw if there are no more events
        if (redraw && al_is_event_queue_empty (event_queue) ) {
            redraw = false;
            al_clear_to_color (al_map_rgb (0,0,0) );

            // make the camera centered over the ship
            cam.x = ship.x - (cam.w / 2);
            cam.y = ship.y - (cam.h / 2);

            for (entity e : theWorld.ents) {
                // rotate the objects based on the camera's angle
                float s = std::sin (ship.angle);
                float c = std::cos (ship.angle);

                float tempx = e.x;
                float tempy = e.y;

                tempx -= ship.x;
                tempy -= ship.y;

                float rotx = tempx * c - tempy * s;
                float roty = tempx * s + tempy * c;

                tempx = rotx + ship.x;
                tempy = roty + ship.y;

                // check if the entity is visible
                if (tempx >= cam.x && tempy >= cam.y && tempx <= cam.x + cam.w && tempy <= cam.y + cam.h) {
                    al_draw_filled_circle (tempx - cam.x, tempy - cam.y, 20, al_map_rgb (200, 0, 0) );
                }
            }

            al_draw_filled_circle (ship.x - cam.x, ship.y - cam.y, 20, al_map_rgb (255, 255, 255) );

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
