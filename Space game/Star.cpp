#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <Star.h>

void Star::init(float pR, float pX, float pY){
	r = pR;
	p = Vec2f(pX, pY);
	srand(time(NULL));
	// TODO: Generate mass by the seed that generates the universe
	m = rand() % 7 + 4;
}

