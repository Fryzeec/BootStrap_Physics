#include "Plane.h"



Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::vec2(0, 1);
}

Plane::Plane(const glm::vec2& normal, float distanceToOrigin, glm::vec4& colour) :
	PhysicsObject(ShapeType::PLANE), m_normal(normal), m_distanceToOrigin(distanceToOrigin), m_colour(colour)
{
	
}

Plane::~Plane()
{
}

void Plane::makeGizmo() 
{
	const float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * -m_distanceToOrigin;
	//glm::vec4 colour{ 1,0,1,0 };

	glm::vec2 parrallel{ m_normal.y, -m_normal.x };

	glm::vec2 start = centerPoint + (parrallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parrallel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, m_colour);
}

void Plane::resolveCollsion(RigidBody * actor2, glm::vec2 contact)
{
	glm::vec2 Normal = this->getNormal();
	glm::vec2 relativVelocity = actor2->getVelocity();

	float elasticity = 0.5f;
	float jTop = -(1 + elasticity) * glm::dot((relativVelocity), Normal);
	float jBottom =	glm::dot(Normal, Normal) * (1 / actor2->getMass());
	float j = jTop / jBottom;

	glm::vec2 force = Normal * j;

	actor2->applyForce(force, contact - actor2->getPosition());
}



