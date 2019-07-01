#include "Game1942App.h"

Game1942App::Game1942App() {

}

Game1942App::~Game1942App() {

}




bool Game1942App::startup() 
{
	
	if (menuState == true && firstpass == true ) 
	{
		//build menu
		setBackgroundColour(0, 0, 0, 1);
		m_menu = new MainMenu();
		m_menu->startup();
		return true;
	}
	else if (menuState == true && firstpass == false) {
		setBackgroundColour(0, 0, 0, 1);
	}
	if (gameState == true)
	{
		if (firstpass == true) {
			m_boss = new Boss();
			m_healthPickUp = new HealthPickUp();
			m_death = new GameOver();
			m_pauseMenu = new PauseMenu();
			m_pauseMenu->StartUp();
			m_bar = new HealthBar(screenWidth * 0.5f, screenHeight - 20, 400, 20);
			//create player
			m_player = new Player();
			//create renderer
			m_2dRenderer = new aie::Renderer2D();
			//create font
			m_font = new aie::Font("./font/consolas.ttf", 32);
			//set background colour
			setBackgroundColour(0, 0.51, 2.55, 0.9);
			//creates clouds
			//allocate pngs to land in background
			for (int i = 0; i < numOfBg; ++i)
			{
				m_land.push_back(new Land());
				m_clouds.push_back(new Clouds());
			}
			//create bullets
			for (int i = 0; i < maxBullets; ++i)
			{
				m_bullet.push_back(new Bullet());
				m_eBullet.push_back(new Bullet());
			}
			//Create Enemies
			for (int i = 0; i < numOfSShips; ++i)
			{
				temp = rand() % 2 + 1;
				switch (temp)
				{
				case 1:
					m_smallShip.push_back(new SmallShip());
					break;
				case 2:
					m_smallShip.push_back(new BigShip());
				};
			}
			for (int i = 0; i < 4; ++i) {
				m_turrets.push_back(new Cannon(i));
			}
			firstpass = false;
		}
		else {
			m_boss->Reset(0);
			bossActive = false;
			for (int i = 0; i < 4; ++i) {
				m_turrets[i]->Reset(i);
			}
			for (int i = 0; i < numOfBg; ++i)
			{
				m_land[i]->Reset();
				m_clouds[i]->Reset();
			}
			//create bullets
			for (int i = 0; i < maxBullets; ++i)
			{
				m_bullet[i]->Reset();
				m_eBullet[i]->Reset();
			}
			//Create Enemies
			for (int i = 0; i < numOfSShips; ++i)
			{
				m_smallShip[i]->Reset(screenWidth, screenHeight);
			}
			setBackgroundColour(0, 0.51, 2.55, 0.9);
			m_player->Reset();
			m_bar->Reset(screenWidth * 0.5f, screenHeight - 20, 400, 20);
			wait = clock() - duration;
			duration = 0;
			enemyState = false;
		}
			return true;
	}
}

void Game1942App::shutdown() 
{
	//checks if game state is active
	if (gameState == true && paused == false && con != true)
	{
		if (m_gameOver == true) {
			delete m_boss;
			m_bullet.~vector();
			m_eBullet.~vector();
			m_smallShip.~vector();
			delete m_bar;
			delete m_menu;
			delete m_pauseMenu;
			delete m_death;
			m_clouds.~vector();
			m_land.~vector();
			delete m_player;
			delete m_font;
			delete m_2dRenderer;
		}
		else if (menuState == false) {
			DeActivate();
		}
	}
	con = false;

}

void Game1942App::update(float deltaTime) 
{


	//checks if main menu is active
	if (menuState == true) {
		m_menu->Menu(gameState, quitState);
		if (gameState == true) 
		{
			menuState = false;
			m_menu->isActive = false;
			startup();
		}
		else if (quitState == true) 
		{
			quit();
		}

	}
	//game state
	aie::Input* input = aie::Input::getInstance();
	if (gameState == true && paused == false && deathState == false)
	{
		duration = (clock() - wait) / (float)CLOCKS_PER_SEC;
		if (duration > bossTimer) {
			bossActive = true;
			enemyState = false;
		}

		duration = (clock() - wait) / (float)CLOCKS_PER_SEC;
		if (duration > startTimer) {
			enemyState = true;
		}

		m_bar->SetValue(m_player->health);

		m_player->immune = false;
		if (bossActive == false) {
			for (int i = 0; i < numOfSShips; ++i)
			{
				if (m_smallShip[i]->isAlive == false)
				{
					m_smallShip[i]->Reset(screenWidth, screenHeight);
				}
			}
		}

		if (bossActive == true) {
			m_boss->Move(deltaTime);
			for (int i = 0; i < m_turrets.size(); ++i) {
				m_turrets[i]->Move(deltaTime, m_player, m_bullet);
			}
		}

		//player fire Bullet one bullet per press
		if (input->wasKeyPressed(aie::INPUT_KEY_SPACE)) {
			for (int i = 0; i < maxBullets; ++i) {
				if (m_bullet[i]->exists != true && m_player->playerFired != true) 
				{
					m_bullet[i]->PlayerFired(m_player);
				}
			}
		}
		m_player->playerFired = false;
		//move each bullet that has been fired
		for (int i = 0; i < maxBullets; ++i) 
		{
			if (m_bullet[i]->exists		 != false) 
			{

				m_bullet[i]->Move(deltaTime);
				//remove bullet if it leaves screen
				if (m_bullet[i]->pos.y > screenHeight) 
				{
					m_bullet[i]->exists   = false;
				}
				//remove bullet if it has collided
				else if (m_bullet[i]->collided == true) 
				{
					m_bullet[i]->exists   = false;
					m_bullet[i]->collided = false;
				}
			}
			//move bullets back to player position while non-existant
			else if (m_bullet[i]->exists != true) 
			{
				m_bullet[i]->pos.x = m_player->pos.x;
				m_bullet[i]->pos.y = m_player->pos.y;
			}
		}

		//move background items
		for (int i = 0; i < numOfBg; ++i) 
		{
			m_clouds[i]->Move(deltaTime, m_clouds[i]->cloudSpeed);
			m_land[i]->Move(deltaTime, m_land[i]->landSpeed);

			for (int j = i + 1; j < numOfBg; ++j) 
			{
				//check if land collides with one another
				if (m_col->Collision(m_land[i], m_land[j]) == true) 
				{
					m_land[j]->pos.x = rand() % screenWidth + 1;
					m_land[j]->pos.y += displacment;
				}
				//check if clouds collide with one another
				if (m_col->Collision(m_clouds[i], m_clouds[j]) == true) 
				{
					m_clouds[j]->pos.x = rand() % screenWidth + 1;
					m_clouds[j]->pos.y += displacment;
				}
			}
		}
		//move the player
		m_player->Move(input, deltaTime);
		m_player->Contain(screenWidth, screenHeight);
		//move Enemy
		for (int i = 0; i < numOfSShips; ++i)
		{
			if(enemyState == true)
			//pause ship flight and move
			if (m_smallShip[i]->isAlive == true && m_smallShip[i]->hasStopped != true)
			{
				if (m_smallShip[i]->pos.y < screenHeight * 0.5)
				{
					m_smallShip[i]->PauseFlight();
				}
				else
					m_smallShip[i]->Move(deltaTime, screenWidth, screenHeight);
			}
			else if (m_smallShip[i]->isAlive == true && m_smallShip[i]->hasStopped == true)
			{
				m_smallShip[i]->Move(deltaTime, screenWidth, screenHeight);
			}
			//check if enemy has fired bullet
			for (int b = 0; b < maxBullets; ++b) 
			{
				for (int j = 0; j < numOfSShips; ++j)
				{
					m_smallShip[j]->WaitToFire();
					if (m_eBullet[b]->exists != true && m_smallShip[j]->hasFired != true)
					{
						m_eBullet[b]->EnemyFired(m_smallShip[j]);
						m_smallShip[j]->hasFired = true;
					}

				}

			}
		}
		for (int b = 0; b < maxBullets; ++b)
		{
			//move each bullet that has been fired
			if (m_eBullet[b]->exists != false)
			{
				m_eBullet[b]->Move(deltaTime);
				//if bullet passes out of bottom of screen remove
				if (m_eBullet[b]->pos.y <= 0)
				{
					m_eBullet[b]->collided = false;
					m_eBullet[b]->exists = false;
					m_eBullet[b]->efire  = false;
				}
			}

		}

		//check collisions if player is not immune
		if (m_player->immune == false)
		{
			for (int i = 0; i < numOfSShips; ++i)
			{
				//check if player collided with enemy
				if(m_smallShip[i]->isAlive == true)
				m_col->CheckPVECollision(m_player, m_smallShip[i], numOfSShips);
			}
			//check for bullet vs enemy collisions
			for (int i = 0; i < numOfSShips; ++i)
			{
				if (m_smallShip[i]->isAlive == true) {
					m_col->CheckBVECollision(m_bullet, m_smallShip[i], maxBullets, numOfSShips, m_player);
				}
				//when enemy dies check to see if health should spawn
				if (m_smallShip[i]->isAlive == false && m_smallShip[i]->collided == true) {
					m_healthPickUp->SpawnHealth(m_smallShip[i]->pos.x, m_smallShip[i]->pos.y);
				}
			}
			//check for player bullet collisions
			m_col->CheckBVPCollision(m_eBullet, m_player, maxBullets);
		}
		m_healthPickUp->Move(deltaTime);
		if (m_col->Collision(m_healthPickUp, m_player) == true) {
			m_player->Heal();
			m_healthPickUp->DeActivate();
			m_player->healed = false;
		}
		//health check
		if (m_player->health <= 0)
		{
			m_player->lives -= 1;
			m_player->health = m_player->maxHealth;
			//enter gameover state
			if (m_player->lives < 0) {
				deathState = true;
			}
		}
		//pause if player presses pause
		if (input->wasKeyPressed(aie::INPUT_KEY_ESCAPE)) {
			paused = true;
		}
		//check screen size
		auto window = glfwGetCurrentContext();
		glfwGetWindowSize(window, &screenWidth, &screenHeight);


		if (bossActive == true) {
			//deactivate bullets
			for (int i = 0; i < maxBullets; ++i)
			{
				m_eBullet[i]->exists = false;
			}
			//deactivate Enemies
			for (int i = 0; i < numOfSShips; ++i)
			{
				m_smallShip[i]->isAlive = false;
				m_smallShip[i]->pos.x = 0;
				m_smallShip[i]->pos.y = 0;
			}
			m_healthPickUp->DeActivate();
			for (int i = 0; i < 4; ++i) {
				if (m_turrets[i]->isAlive == false && m_turrets[i]->toggled == false) {
					cannonDestroyed += 1;
					m_turrets[i]->toggled == true;
				}
			}
			if (cannonDestroyed == 4) {
				m_player->score += 2000;
				deathState == true;
			}
		}
	}
	//enter gameover state when player has no lives left
	if (deathState == true)
	{
		isDead = m_death->DeathMenu();
		if (isDead == true) {
			shutdown();
			startup();
			deathState = false;
		}
		if (isDead == false) {
			shutdown();
			gameState = false;
			menuState = true;
			startup();
			deathState = false;
		}

	}

	if (paused == true)
	{
		//initialise pause menu
		m_pauseMenu->Pause(paused, quitState);
		//if continue is selected
		if (paused == false && quitState != true) 
		{
			con = true;
		}
		//if exit is selected
		else if (quitState == true) 
		{
			shutdown();
			gameState = false;
			menuState = true;
			startup();
			quitState = false;
		}	
	}
}

void Game1942App::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	//check if menu should be drawn
	if (menuState == true)
	{
		m_menu->draw();
	}
	//check if gamestate should be drawn
	else if (gameState == true)
	{
		//check if pause menu should be drawn
		if (paused == true)
		{
			m_pauseMenu->Draw(screenWidth, screenHeight);
		}
		if (deathState == true) {
			m_bar->Draw(m_2dRenderer);
			m_death->Draw(screenWidth, screenHeight, m_player->score);
		}
		m_healthPickUp->Draw();
		//draw boss when active
		if (bossActive == true) {
			m_boss->Draw();
			for (int i = 0; i < m_turrets.size(); ++i) {
				m_turrets[i]->Draw();
			}
		}
		// begin drawing sprites
		m_2dRenderer->begin();
		// draw your stuff here!
		
		//draw health bar
		m_bar->Draw(m_2dRenderer);
		
		//draw bullets
		for (int i = 0; i < maxBullets; ++i)
		{
			if (m_bullet[i]->exists != false)
			{
				m_2dRenderer->drawSprite(m_bullet[i]->texture,
					m_bullet[i]->pos.x,
					m_bullet[i]->pos.y,
					m_bullet[i]->size,
					m_bullet[i]->size);
			}
			if (m_eBullet[i]->exists != false && enemyState == true)
			{
				m_2dRenderer->drawSprite(m_eBullet[i]->texture,
					m_eBullet[i]->pos.x,
					m_eBullet[i]->pos.y,
					m_eBullet[i]->size,
					m_eBullet[i]->size);
			}
		}
		//draw lives
		m_player->Draw(m_2dRenderer, m_font, screenWidth, screenHeight);
		//Draw Enemy 
		if (enemyState == true)
		{
			for (int i = 0; i < numOfSShips; ++i)
			{
				if (m_smallShip[i]->isAlive == true)
				{
					m_2dRenderer->drawSprite(m_smallShip[i]->texture,
						m_smallShip[i]->pos.x,
						m_smallShip[i]->pos.y,
						m_smallShip[i]->pos.w,
						m_smallShip[i]->pos.h, 0, 2);
				}
			}
		}
		for (int i = 0; i < numOfBg; ++i)
		{
			//draw clouds	
			m_2dRenderer->drawSprite(m_clouds[i]->texture,
				m_clouds[i]->pos.x,
				m_clouds[i]->pos.y, 300, 300, 0, 99);
			//draw land
			m_2dRenderer->drawSprite(m_land[i]->texture,
				m_land[i]->pos.x,
				m_land[i]->pos.y, 300, 300, 0, 100);
		}

		// output some text, uses the last used colour

		// done drawing sprites
		m_2dRenderer->end();


	}
}

void Game1942App::DeActivate() {
	for (int i = 0; i < numOfBg; ++i)
	{
		m_land[i]->Active = false;
		m_clouds[i]->Active = false;
	}
	//deactivate bullets
	for (int i = 0; i < maxBullets; ++i)
	{
		m_bullet[i]->exists = false;
		m_eBullet[i]->exists = false;
	}
	//deactivate Enemies
	for (int i = 0; i < numOfSShips; ++i)
	{
		m_smallShip[i]->isAlive = false;
	}
	m_bar->isActive = false;

}
