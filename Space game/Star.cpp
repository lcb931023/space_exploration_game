#include <iostream>
#include <stdlib.h>
#include <Star.h>
#include <Box2D/Box2D.h>



Star::Star(b2World &pWorld, float pR, float pX, float pY, float pM)
	:world(pWorld)
{
	r = pR;
	p = Vec2f(pX, pY);
	// TODO: Generate mass by the seed that generates the universe
	m = pM;

	// Generate box2d object
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(p.x(), p.y());
	b2Body = world.CreateBody(&bd);
	b2CircleShape cs;
	cs.m_radius = r;
	b2FixtureDef fd;
	fd.shape = &cs;
	b2Body->CreateFixture(&fd);
}