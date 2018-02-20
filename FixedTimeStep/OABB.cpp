#include "OABB.h"


OABB::OABB(glm::vec2 position, glm::vec2 velocity, float mass, glm::vec4 colour, glm::vec2 extents) 
	: RigidBody(ShapeType::OABB, position, velocity, 0, mass)
{
	m_colour = colour;
	m_extents = extents;
	m_localX = glm::vec2(1,0);
	m_localY = glm::vec2(0,1);
}

OABB::~OABB()
{
}

void OABB::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	//store the local axis
	float cs = cosf(m_rotation);
	float sn = sinf(m_rotation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

void OABB::makeGizmo()
{
	//if only using rotation
	//glm::mat4 transform = glm::rotate(m_rotation, glm::vec3(0,0,1));
	//aie::Gizmos::add2DAABBFILLED(getCenter(), m_extents, m_colour, &transform);

	//draw using local axis
	glm::vec2 p1 = m_position - m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p2 = m_position + m_localX * m_extents.x - m_localY * m_extents.y;
	glm::vec2 p3 = m_position - m_localX * m_extents.x + m_localY * m_extents.y;
	glm::vec2 p4 = m_position + m_localX * m_extents.x + m_localY * m_extents.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

bool OABB::checkCollision(PhysicsObject * pOther)
{
	return false;
}
