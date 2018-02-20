#pragma once
#include "RigidBody.h"
class OABB : public RigidBody
{
public:
	OABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents);
	~OABB();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject* pOther);


protected:
	glm::vec2 m_extents; //half the lengths
	glm::vec4 m_colour;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

