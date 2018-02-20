#pragma once
#include "glm/glm.hpp"
#include "PhysicsObject.h"
#include "RigidBody.h"
#include "Sphere.h"
class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(const glm::vec2& normal, float distanceToOrigin, glm::vec4& colour);
	~Plane();

	virtual void fixedUpdate(glm::vec2 gravity, float dt) override {};
	virtual void debug() override {};
	virtual void makeGizmo();

	void resolveCollsion(RigidBody* actor2, glm::vec2 contact);

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_colour;
	
};

