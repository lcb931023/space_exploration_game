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
b2Body* star = NULL;

float fClamp (float pf, float min, float max) {
	if (pf >= max) pf = max;
	if (pf <= min) pf = min;
	return pf;
}

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
		Star tStar;
		r = 30 + 20 * fClamp(p.noise(iP * 0.036, iP * 0.063, (iP+1) * 0.086), -1, 1);
		x =	400 + 400 * fClamp(p.noise(iP * 0.03, iP * 0.05, (iP+1) * 0.02), -1, 1);
		y =	300 + 300 * fClamp(p.noise(iP * 0.05, iP * 0.03, (iP+1) * 0.06), -1, 1);
		tStar.init(r, x, y);
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
	// Procedual Texture
	/*
	p.setPersistence(.6);
	for (int i = 0; i<800; i++ ){
		for (int j = 0; j<600; j++){
			//float n = fClamp(abs(p.noise(i * .007,j * .008, zPerlin) * 255), 0, 255);
			float n = abs(p.noise(i * .2,j * .2, zPerlin) * 255);
			al_draw_filled_circle(i + 2, j + 2, 1, al_map_rgb(n, n, n));
		}
	}
	*/

}
/*** Star texture Charts 
Sun's inner texture: setPersistence(0.6), noise(x * 0.2,y * 0.2, z)
Moon & Mercury: setPersistence(1), noise(x * 0.007,y * 0.008, z)
Jupiter: setPersistence(0.1), noise(x * 0.002,y * 0.02, z)
Venus: setPersistence(0.6), noise(x * 0.002,y * 0.02, z), clamp it
***/

void makeShip(){
	// Body Definition
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(300.0f, 200.0f);
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
	//ship->SetLinearVelocity(b2Vec2(0.0f, 75.0f));
	// Prepare for rendering
	bm_ship = al_create_bitmap(d_width*PIX_METER, d_height*PIX_METER);
	al_set_target_bitmap(bm_ship);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_set_target_bitmap(al_get_backbuffer(display));
}
void makeStar(){
	// Body Definition
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(500.0f, 300.0f);
	star = world.CreateBody(&bd);
	// Shape Definition
	b2CircleShape cs;
	float radius = 40.0f;
	cs.m_radius = radius;
	// Fixture Definition
	b2FixtureDef fd;
	fd.shape = &cs;
	// Connect fd that has shape and density to the star body
	star->CreateFixture(&fd);
}

void applyGravityToShip(){
	// F = GMm/R^2
	b2Vec2 gravityPull = star->GetPosition() - ship->GetPosition();
	float starMass = 10.0f;
	float shipMass = 1.0f;
	float forceMag = starMass * shipMass / ( pow(gravityPull.Length(), 2.0) );
	gravityPull.Normalize();
	gravityPull *= forceMag;
	gravityPull *= 100000000.0f;
	ship->ApplyForce(gravityPull, star->GetPosition(), true);
}

void updateSimulation(){
	applyGravityToShip();
	world.Step(timeStep, velocityIterations, positionIterations);
	world.ClearForces();
}

void renderSimulation(){
	// Draw the ship
	b2Vec2 s_pos = ship->GetPosition();
	float32 angle = ship->GetAngle();
	float s_x = s_pos.x;
	float s_y = s_pos.y;
	float midx = al_get_bitmap_width(bm_ship)/2;
	float midy = al_get_bitmap_height(bm_ship)/2;
	al_draw_rotated_bitmap(bm_ship, midx, midy, s_x*PIX_METER, s_y*PIX_METER, angle, 0);
	// draw star
	al_draw_filled_circle (star->GetPosition().x, star->GetPosition().y, 40.0f, al_map_rgb (255, 255, 255) );
}

int main (int argc, char** argv) {
	//B2_NOT_USED(argc);
	//B2_NOT_USED(argv);

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
		al_destroy_bitmap (bm_ship);
        al_destroy_display (display);
        al_destroy_timer (timer);
        return -1;
    }

    // register event sources for events to come from
    al_register_event_source (event_queue, al_get_display_event_source (display) );
    al_register_event_source (event_queue, al_get_timer_event_source (timer) );

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