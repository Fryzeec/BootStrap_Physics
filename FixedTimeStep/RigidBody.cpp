#include "RigidBody.h"



RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass)
	: PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_rotation = rotation;
	m_angularVelocity = 0.0f;
	m_isKinematic = false;
	m_elasticity = 1.0f;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		return;
	}

	//remeber when applying the force of gravity, mass cancels out
	m_velocity += gravity * timeStep;
	m_position += m_velocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;


	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD) 
			m_velocity = glm::vec2(0, 0);

	if (glm::abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
		    m_angularVelocity = 0;

}

void RigidBody::debug()
{
}

void RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / (m_moment);
}

void RigidBody::resolveCollsion(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	//find the vector between their centres, or use the probided diretion
	//of force

	glm::vec2 Normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->m_position - m_position);

	//get the vector perpedicular to the collision normal
	glm::vec2 perp(Normal.y, -Normal.x);

	//determin the total velocity of the contact points for the two objects,
	//for both linear and rotational

	//'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	//velocity of the contact point on this object
	float v1 = glm::dot(m_velocity, Normal) - r1 * m_angularVelocity;
	//velocity of the contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, Normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) //they're moving closer to each other
	{
		//calculate the effective mass at contact point for each object
		//ie how much the contact point will move due to the force applied
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r2) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * Normal;

		//apply equal and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);
	}
}
