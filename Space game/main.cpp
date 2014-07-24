#include <iostream>
#include <stdlib.h>     /* abs */
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <Vec2.h>
#include <FractalNoise.h>
#include <Star.h>
#include <Box2D/Box2D.h>

const float FPS = 60;
const int PIX_METER = 1;
FractalNoise p;
float zPerlin = 0.7;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *bm_ship = NULL;

// Box2d
b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);
float32 timeStep = 1.0f / FPS;
int32 velocityIterations = 10;
int32 positionIterations = 10;
b2Body* ship = NULL;
Star* star = NULL;

float fClamp (float pf, float min, float max) {
	if (pf >= max) pf = max;
	if (pf <= min) pf = min;
	return pf;
}
/*
bool starTooClose(Star s1, Star s2) {
	float passageW = 10;
	float dist = (s1.p - s2.p).magnitude(); 
	if ( dist < (s1.r + s2.r + passageW) )
	{
		return true;
	}
	return false;
}

void drawStars () {
	float x;
	float y;
	float r;
	const int size = 12;
	int iP = 1;
	std::vector<Star> stars(size);
	// Procedual Position & Radius
	p.setPersistence(1);
	for (int i=0; i<size; i++, iP++)
	{
		r = 30 + 20 * fClamp(p.noise(iP * 0.036, iP * 0.063, (iP+1) * 0.086), -1, 1);
		x =	400 + 400 * fClamp(p.noise(iP * 0.03, iP * 0.05, (iP+1) * 0.02), -1, 1);
		y =	300 + 300 * fClamp(p.noise(iP * 0.05, iP * 0.03, (iP+1) * 0.06), -1, 1);
		Star tStar(r, x, y);
		stars[i] = tStar;
		for (int j=0; j<i; j++)
		{
			if ( starTooClose(stars[i], stars[j]) )
			{
				i--;
				break;
			}
		}
	}
	for (int i=0; i<size; i++)
	{
		al_draw_filled_circle (stars[i].p.x(), stars[i].p.y(), stars[i].r, al_map_rgb (255, 255, 255) );
	}
}
*/
/*** Star texture Charts 
Sun's inner texture: setPersistence(0.6), noise(x * 0.2,y * 0.2, z)
Moon & Mercury: setPersistence(1), noise(x * 0.007,y * 0.008, z)
Jupiter: setPersistence(0.1), noise(x * 0.002,y * 0.02, z)
Venus: setPersistence(0.6), noise(x * 0.002,y * 0.02, z), clamp it

// Procedual Texture
	
	p.setPersistence(.6);
	for (int i = 0; i<800; i++ ){
		for (int j = 0; j<600; j++){
			//float n = fClamp(abs(p.noise(i * .007,j * .008, zPerlin) * 255), 0, 255);
			float n = abs(p.noise(i * .2,j * .2, zPerlin) * 255);
			al_draw_filled_circle(i + 2, j + 2, 1, al_map_rgb(n, n, n));
		}
	}

***/

void makeShip(){
	// Body Definition
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(300.0f, 250.0f);
	ship = world.CreateBody(&bd);
	// Shape Definition
	b2PolygonShape ps;
	float d_width = 20.0f;
	float d_height = 40.0f;
	ps.SetAsBox(d_width/2,d_height/2);
	// Fixture Definition
	b2FixtureDef fd;
	fd.shape = &ps;
	fd.density = 1;
	// Connect fd that has shape and density to the ship body
	ship->CreateFixture(&fd);
	// [TEMP] give it some motion
	//ship->SetLinearVelocity(b2Vec2(75.0f, -50.0f));
	// Prepare for rendering
	bm_ship = al_create_bitmap(d_width*PIX_METER, d_height*PIX_METER);
	al_set_target_bitmap(bm_ship);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_set_target_bitmap(al_get_backbuffer(display));
}
void makeStar(){
	star = new Star(world, 40.0f, 500.0f, 300.0f, 20.0f);
}

void moveShip() {
	// move ship

}

void applyGravityToShip(){
	// F = GMm/R^2
	b2Vec2 gravityPull = star->b2Body->GetPosition() - ship->GetPosition();
	float starMass = 10.0f;
	float shipMass = 1.0f;
	float forceMag = starMass * shipMass / ( pow(gravityPull.Length(), 2.0) );
	gravityPull.Normalize();
	gravityPull *= forceMag;
	gravityPull *= 50000000.0f;
	ship->ApplyForce(gravityPull, star->b2Body->GetPosition(), true);
}

void updateSimulation(){
	moveShip();
	applyGravityToShip();
	world.Step(timeStep, velocityIterations, positionIterations);
	world.ClearForces();
}

void renderSimulation(){
	// [TODO] Camera Integreation
	// Draw the ship
	b2Vec2 s_pos = ship->GetPosition();
	float32 angle = ship->GetAngle();
	float s_x = s_pos.x;
	float s_y = s_pos.y;
	float midx = al_get_bitmap_width(bm_ship)/2;
	float midy = al_get_bitmap_height(bm_ship)/2;
	al_draw_rotated_bitmap(bm_ship, midx, midy, s_x*PIX_METER, s_y*PIX_METER, angle, 0);
	// draw star
	al_draw_filled_circle (star->b2Body->GetPosition().x, star->b2Body->GetPosition().y, 40.0f, al_map_rgb (255, 255, 255) );
}

void handleKeyDown(int pKeyCode) {
	switch (pKeyCode) {
        case ALLEGRO_KEY_W:
            // Up
			ship->ApplyForce(b2Vec2(0.0f, -1000000.0f), ship->GetPosition(), true);
            break;
        case ALLEGRO_KEY_S:
            // Down
			ship->ApplyForce(b2Vec2(0.0f, 1000000.0f), ship->GetPosition(), true);
            break;
        case ALLEGRO_KEY_A:
            // Left
			ship->ApplyForce(b2Vec2(-1000000.0f, 0.0f), ship->GetPosition(), true);
            break;
        case ALLEGRO_KEY_D:
            // Right
			ship->ApplyForce(b2Vec2(1000000.0f, 0.0f), ship->GetPosition(), true);
            break;
        case ALLEGRO_KEY_Q:
            // Rot L
            break;
        case ALLEGRO_KEY_E:
            // Rot R
            break;
    }
}

void handleKeyUp(int pKeyCode) {
	switch (pKeyCode) {
		case ALLEGRO_KEY_W:
			// reset U/D
			break;
		case ALLEGRO_KEY_S:
			// reset U/D
			break;
		case ALLEGRO_KEY_A:
			// reset L/R
			break;
		case ALLEGRO_KEY_D:
			// reset L/R
			break;
		case ALLEGRO_KEY_Q:
			// reset rot
			break;
		case ALLEGRO_KEY_E:
			// reset rot
			break;
	}
}

int main (int argc, char** argv) {
	//B2_NOT_USED(argc);
	//B2_NOT_USED(argv);

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
		al_destroy_bitmap (bm_ship);
        al_destroy_display (display);
        al_destroy_timer (timer);
        return -1;
    }

    // register event sources for events to come from
    al_register_event_source (event_queue, al_get_display_event_source (display) );
    al_register_event_source (event_queue, al_get_timer_event_source (timer) );
	al_register_event_source (event_queue, al_get_keyboard_event_source () );

	makeShip();
	makeStar();
	 
    // clear the screen and start the FPS timer
    al_clear_to_color (al_map_rgb (0,0,0) );
	//drawStars();
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
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			handleKeyDown(ev.keyboard.keycode);
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			handleKeyUp(ev.keyboard.keycode);
		}
		updateSimulation();
        // redraw if there are no more events
        if (redraw && al_is_event_queue_empty (event_queue) ) {
            redraw = false;
            al_clear_to_color (al_map_rgb (0,0,0) );
			renderSimulation();
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
