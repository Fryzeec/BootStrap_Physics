#pragma once
#include "RigidBody.h"
class oabb : public RigidBody
{
public:
	oabb(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents, float width, float height);
	~oabb();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	glm::vec2 getLocalX() { return m_localX; }

	glm::vec2 getLocalY() { return m_localY; }

	glm::vec2 getExtents() { return m_extents; }

	float getHeight() { return m_height; }
	float getWidth() { return m_width; }

	float getMoment() { return m_moment = 1.0f / 12.0f * m_mass * m_width * m_height; }

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);


protected:
	glm::vec2 m_extents; //half the lengths
	glm::vec4 m_colour;

	float m_width;
	float m_height;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

