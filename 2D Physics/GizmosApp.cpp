#include "GizmosApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

_2D_PhysicsApp::_2D_PhysicsApp() {

}

_2D_PhysicsApp::~_2D_PhysicsApp() {

}

bool _2D_PhysicsApp::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	//                2D          3D
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);


	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	return true;
}

void _2D_PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void _2D_PhysicsApp::update(float deltaTime) {
	aie::Gizmos::clear();

	// input example
	aie::Input* input = aie::Input::getInstance();

	//aie::Gizmos::add2DCircle(glm::vec2(0, 0), 10.0f, 32, glm::vec4(1, 0, 0, 1));

	//Breakout
	//colours
	static const glm::vec4 colours[] = {
		glm::vec4(1,0,0,1), glm::vec4(0,1,0,1),
		glm::vec4(0,0,1,1), glm::vec4(0.8f,0,0.5f,1),
		glm::vec4(1,1,1,1)
	};
	//Dimentions
	static const int rows = 5;
	static const int columns = 10;
	static const int hSpace = 1;
	static const int vSpace = 1;

	static const glm::vec2 scrExtents(100, 50);
	static const glm::vec2 boxExtents(7, 3);
	static const glm::vec2 startPos
		//Blocks shift
	(-(columns/2) * ((boxExtents.x * 2) + vSpace) + boxExtents.x + (vSpace / 2.0f),
		scrExtents.y - ((boxExtents.y * 2) + hSpace));

	//Draw the grid of blocks
	glm::vec2 pos;
	for (int y = 0; y < rows; y++)
	{
		pos = glm::vec2(startPos.x, startPos.y - (y* ((boxExtents.y * 2) + hSpace)));
		for (int x = 0; x < columns; x++)
		{
			aie::Gizmos::add2DAABBFilled(pos, boxExtents, colours[y]);
			pos.x += (boxExtents.x * 2) + vSpace; 
		}
	}

	//Draw ball
	aie::Gizmos::add2DCircle(glm::vec2(0, -35), 3, 12, glm::vec4(1, 1, 0, 1));
	//Draw the players paddle
	aie::Gizmos::add2DAABBFilled(glm::vec2(0, -40), glm::vec2(12, 2), glm::vec4(1, 0, 1, 1));

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void _2D_PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	float aspectRatio = (float)getWindowWidth() / getWindowHeight();
	//                                    L        R       B                     T
	aie::Gizmos::draw2D(glm::ortho<float>(-100.0f, 100.0f, -100.0f / aspectRatio, 100.0f / aspectRatio, -1.0f, 1.0f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}