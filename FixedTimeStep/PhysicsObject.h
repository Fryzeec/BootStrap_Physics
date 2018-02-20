#pragma once
#include <glm\glm.hpp>
#include <Gizmos.h>
enum ShapeType
{
	PLANE = 0,
	SPHERE,
	OABB,
	SHAPE_COUNT
};

class PhysicsObject
{
public:

	PhysicsObject();
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};
	~PhysicsObject();

	int const getShapeID() {return m_shapeID; }

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

private:
	ShapeType m_shapeID;

};