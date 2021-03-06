#pragma once
#include <Input.h>
#include <Texture.h>
#include "Application.h"
#include <Renderer2D.h>
#include <Font.h>
#include <iostream>
#include  <MathLib.h>
class MainMenu
{
private:

	float screenX			= 100;
	float screenY			= 720;
	float screenW			= 1280;
	float planePositionX	= 900;
	bool startSelected		= true;
	bool startMouse			= false;
	bool exitSelected		= false;
	bool exitmouse		    = false;
	int switchy = 0;
	int enterCounter = 0;
	Vector2* mousePosition;
	void MouseOver();
public:
	MainMenu();
	void startup();
	void shutdown();
	void Menu(bool &state, bool &quit);
	void Reset();
	void draw(aie::Renderer2D* m_renderer);
	 ~MainMenu();
	 bool isActive = false;
	 bool instructions = false;
	 aie::Texture*		m_plane;
	 aie::Font*			m_font;
	 aie::Font*			m_titleFont;
	 aie::Input*		m_input;
};

