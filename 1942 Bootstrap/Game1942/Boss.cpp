#include "Boss.h"



Boss::Boss()
{
	m_renderer	= new aie::Renderer2D();
	m_texture	= new aie::Texture("./textures/BossBase.png");
}


Boss::~Boss()
{
}


void Boss::Move(float deltaTime)
{
	//move from starting position to inposition
	if (inPosition == false) {
		vector2 downDistance = stoppingPointOne - pos;
		downDistance.x = 0;
		magOne = downDistance.magnitued();
		if (magOne < 1)
		{
			//inPosition = true;
			right = true;
			inPosition = true;
		}

		else if (magOne > 0) 
		{
			pos += downDistance.normalise(downDistance) * 200 * deltaTime;
		}

	}

	//move to right when in position
	if (right == true) {

		vector2 rightDistance = rightscreen - pos;
		rightDistance.y = 0;
		magOne = rightDistance.magnitued();
		if (magOne < 1)
		{
			right = false;
			left = true;
		}
		else if (magOne > 0) {
			pos += rightDistance.normalise(rightDistance) * 200 * deltaTime;
		}

	}

	//move to left if in position
	else if (left == true)
	{

		vector2 leftDistance = leftscreen - pos;
		leftDistance.y = 0;
		magOne = leftDistance.magnitued();
		if (magOne < 1)
		{
			left = false;
			right = true;
		}

		else if (magOne > 0) {
			pos += leftDistance.normalise(leftDistance) * 200 * deltaTime;
		}
	}
		
}

void Boss::Draw()
{
	m_renderer->begin();
	m_renderer->drawSprite(m_texture, pos.x, pos.y,0,0,0,50);
	m_renderer->end();
}

void Boss::Reset(int i) {
	inPosition = false;
	pos.x = m_xPosition;
	pos.y = m_startingY;
}