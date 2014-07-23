#include <Vec2.h>
#include <Box2D/Box2D.h>
class Star {
public:
	Star(b2World&, float, float, float, float);
	~Star(){};

	float r;
	Vec2f p;
	float m;
	b2Body* b2Body;

private:
	b2World &world;
};