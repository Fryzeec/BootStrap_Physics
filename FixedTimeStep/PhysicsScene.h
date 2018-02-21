#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "PhysicsObject.h"
#include <vector>
#include <algorithm>
#include <Gizmos.h>
#include "RigidBody.h"
#include <list>
#include "Sphere.h"
#include "Plane.h"
#include "OABB.h"
class PhysicsScene
{
public:

	PhysicsScene();
	~PhysicsScene();
	
	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();
	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollision();

	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	static bool plane2OABB(PhysicsObject*, PhysicsObject*);
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);
	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	static bool sphere2OABB(PhysicsObject*, PhysicsObject*);
	static bool OABB2Plane(PhysicsObject*, PhysicsObject*);
	static bool OABB2sphere(PhysicsObject*, PhysicsObject*);
	static bool OABB2OABB(PhysicsObject*, PhysicsObject*);
protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};

