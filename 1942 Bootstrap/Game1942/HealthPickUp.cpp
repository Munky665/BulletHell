#include "HealthPickUp.h"



HealthPickUp::HealthPickUp()
{
	texture = new aie::Texture("./textures/healthpickup.png");
	renderer = new aie::Renderer2D();
}


HealthPickUp::~HealthPickUp()
{
}
//moves health pickup down the screen if it exists.
void HealthPickUp::Move(float deltaTime) 
{
	if (exists == true) {
		pos.y -= travelSpeed * deltaTime;

		if (pos.y < 0) {
			exists = false;
		}
	}
}
//spawn health if random number matches at destroyed enemy position
void HealthPickUp::SpawnHealth(float x, float y) 
{
	temp = rand() % variance + 1;
	if (temp == 6 && exists == false) {
		exists = true;
		pos.x = x;
		pos.y = y;
	}

}
//draws health
void HealthPickUp::Draw(aie::Renderer2D* renderer) 
{
	if (exists == true)
	{
		renderer->drawSprite(texture, pos.x, pos.y, size.x, size.y, 0, 49);
	}
}
//resets health position when picked up
void HealthPickUp::DeActivate()
{
	exists = false;
	pos.x = 0;
	pos.y = 0;

}

