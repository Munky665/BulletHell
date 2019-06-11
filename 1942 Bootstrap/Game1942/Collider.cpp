#include "Collider.h"



Collider::Collider()
{
}


Collider::~Collider()
{
}

bool Collider::Collision(Background Bg1, Background Bg2)
{
	//If the leftmost or rightmost point of the first background lies somewhere inside the second, continue.
	if ((Bg1.pos.x >= Bg2.pos.x && Bg1.pos.x <= (Bg2.pos.x + Bg2.pos.w)) ||
		((Bg1.pos.x + Bg1.pos.w) >= Bg2.pos.x && (Bg1.pos.x + Bg1.pos.w) <= (Bg2.pos.x + Bg2.pos.w)))
	{
		//check y axis:
		if ((Bg1.pos.y >= Bg2.pos.y && Bg1.pos.y <= (Bg2.pos.y + Bg2.pos.h)) ||
			((Bg1.pos.y + Bg1.pos.h) >= Bg2.pos.y && (Bg1.pos.y + Bg1.pos.h) <= (Bg2.pos.y + Bg2.pos.h)))
		{
			//The backgrounds appear to overlap.
			return true;
		}
	}
	//The the backgrounds do not overlap:
	return false;
}

bool Collider::Collision(Player* player, Enemy* enemy, int size)
{
	for (int i = 0; i < size; ++i) {
		//If the leftmost or rightmost point of the player lies somewhere inside the Enemy, continue.
		if	((player->pos.x >= enemy->pos.x && player->pos.x <= (enemy->pos.x + enemy->pos.w)) ||
			((player->pos.x + player->pos.w) >= enemy->pos.x && (player->pos.x + player->pos.w) <= (enemy->pos.x + enemy->pos.w))) 
		{
			//check y axis:
			if	((player->pos.y >= enemy->pos.y && player->pos.y <= (enemy->pos.y + enemy->pos.h)) ||
				((player->pos.y + player->pos.h) >= enemy->pos.y && (player->pos.y + player->pos.h) <= (enemy->pos.y + enemy->pos.h)))
			{
				//The player and enemy appear to overlap.
				enemy->collided = true;
				return true;
			}
		}
	}
	//The sprites do not overlap:
	return false;
}
bool Collider::Collision(Bullet* b, Enemy* enemy, int size) 
{
	for (int i = 0; i < size; ++i) 
	{
		//If the leftmost or rightmost point of the Bullet lies somewhere inside the Enemy, continue.
		if ((b->pos.x >= enemy->pos.x && b->pos.x <= (enemy->pos.x + enemy->pos.w)) ||
			((b->pos.x + b->pos.w) >= enemy->pos.x && (b->pos.x + b->pos.w) <= (enemy->pos.x + enemy->pos.w)))
		{
			//check y axis:
			if ((b->pos.y >= enemy->pos.y && b->pos.y <= (enemy->pos.y + enemy->pos.h)) ||
				((b->pos.y + b->pos.h) >= enemy->pos.y && (b->pos.y + b->pos.h) <= (enemy->pos.y + enemy->pos.h))) 
			{
				//The bullet and enemy appear to overlap.
				enemy->collided = true;
				return true;
			}
		}
	}
	//The sprites do not overlap:
	return false;
}

bool Collider::Collision(Bullet* b, Player* p) 
{
		//If the leftmost or rightmost point of the bullet lies somewhere inside the player, continue.
		if ((b->pos.x >=p->pos.x && b->pos.x <= (p->pos.x + p->pos.w)) ||
			((b->pos.x + b->pos.w) >= p->pos.x && (b->pos.x + b->pos.w) <= (p->pos.x + p->pos.w)))
		{
			//check y axis:
			if ((b->pos.y >= p->pos.y && b->pos.y <= (p->pos.y +p->pos.h)) ||
				((b->pos.y + b->pos.h) >= p->pos.y && (b->pos.y + b->pos.h) <= (p->pos.y + p->pos.h)))
			{
				//The bullet and player appear to overlap.
				b->collided = true;
				return true;
			}
		}
	//The sprites do not overlap:
	return false;
}

