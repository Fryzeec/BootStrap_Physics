#pragma once

#include "Application.h"
#include "Renderer2D.h"

class _2D_PhysicsApp : public aie::Application {
public:

	_2D_PhysicsApp();
	virtual ~_2D_PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};