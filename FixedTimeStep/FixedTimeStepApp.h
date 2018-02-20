#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

class FixedTimeStepApp : public aie::Application {
public:

	FixedTimeStepApp();
	virtual ~FixedTimeStepApp();

	void setUpConinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity, int x, int y);

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene* m_physicsScene;
};