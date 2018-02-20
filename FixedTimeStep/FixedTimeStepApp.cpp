#include "FixedTimeStepApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"

FixedTimeStepApp::FixedTimeStepApp() {

}

FixedTimeStepApp::~FixedTimeStepApp() {

}

bool FixedTimeStepApp::startup() {
	//Increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);


	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -50));
	m_physicsScene->setTimeStep(0.01f);

	//Plane
	Plane* plane1 = new Plane(glm::vec2(-1 , 0), 70, glm::vec4(1, 0, 0,1));
	Plane* plane2 = new Plane(glm::vec2(1, 0), 70, glm::vec4(1, 0, 0, 1));
	Plane* plane3 = new Plane(glm::vec2(0, 1), 30, glm::vec4(1, 0, 0, 1));
	Plane* plane4 = new Plane(glm::vec2(0, -1), 30, glm::vec4(1, 0, 0, 1));

	m_physicsScene->addActor(plane1);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
	m_physicsScene->addActor(plane4);

	//Crash balls
	Sphere* ball1 = new Sphere(glm::vec2(-40, 3), glm::vec2(100, 0), 3.0f, 5, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(40, -3), glm::vec2(-100, 0), 3.0f, 5, glm::vec4(0, 1, 0, 1));

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);

	ball1->applyForce(glm::vec2(50, 0), ball1->getPosition());
	ball2->applyForce(glm::vec2(-45, 0), ball2->getPosition());

	
	return true;
}

void FixedTimeStepApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void FixedTimeStepApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance(); 
	
	aie::Gizmos::clear();
	//Upadates physics
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void FixedTimeStepApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}