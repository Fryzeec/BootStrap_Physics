#pragma once
#include "PhysicsObject.h"
const float MIN_LINEAR_THRESHOLD = 0.01f;
const float MIN_ROTATION_THRESHOLD = 0.1f;
#include <iostream>
class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();

	void applyForce(glm::vec2 force, glm::vec2 pos);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;
	void resolveCollsion(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal = nullptr);

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 setPosition(glm::vec2 position) { return m_position = position; }

	float getRotation() { return m_rotation; }
	float setRotation(float rotation) { return m_rotation = rotation ; }

	glm::vec2 getVelocity() { return m_velocity; }
	glm::vec2 setVelocity(glm::vec2 velocity) { return m_velocity = velocity; }

	float getMass() { return m_mass; }
	float setMass(float mass) { return m_mass = mass; }

	float getLinearDrag() { return m_linearDrag; }
	float setLinearDrag(float linearDrag) { return linearDrag = m_linearDrag; }

	float getAngularDrag() { return m_angularDrag; }
	float setAngularDrag(float angularDrag) { return angularDrag = m_angularDrag; }

	float getElasticity() { return m_elasticity; }
	float setElasticity(float elasticity) { return elasticity = m_elasticity; }

	bool getisKinematic() { return m_isKinematic; }
	bool setisKinematic(bool isKinematic) { return isKinematic = m_isKinematic; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation; // 2D so we only need a single float to represent our rotation
	float m_angularVelocity;
	float m_moment;
	float m_linearDrag = 0.3f;
	float m_angularDrag = 0.3f;
	float m_elasticity;
	bool m_isKinematic;
};

