#include "PhysicsScene.h"



PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{

}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere,
	PhysicsScene::plane2OABB, PhysicsScene::sphere2Plane, 
	PhysicsScene::sphere2Sphere, PhysicsScene::sphere2OABB,
	PhysicsScene::OABB2Plane, PhysicsScene::OABB2sphere,
	PhysicsScene::OABB2OABB
};

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

	//need to check for collisions against all obj except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			//using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				//did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	return sphere2Plane(obj2, obj1);
}

bool PhysicsScene::plane2OABB(PhysicsObject *obj1, PhysicsObject *obj2)
{
	return OABB2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Plane(PhysicsObject * obj1, PhysicsObject *obj2)
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		//distance from the obj1 to obj 2
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();

		//if we are behind plane then we flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			float overlap = intersection;
			glm::vec2 Normal = plane->getNormal();
			glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			sphere->setPosition(sphere->getPosition() + (overlap * Normal * -1.0f));

			//Resolve collision
			plane->resolveCollsion(sphere, contact);

			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * obj1, PhysicsObject * obj2)
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float distance = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		if (distance < sphere1->getRadius() + sphere2->getRadius())
		{
			float overlap = sphere1->getRadius() + sphere2->getRadius() - distance;
			glm::vec2 Normal = glm::normalize(sphere1->getPosition() - sphere2->getPosition());
			sphere1->setPosition(sphere1->getPosition() + (overlap * Normal * 0.5f));
			sphere2->setPosition(sphere2->getPosition() - (overlap * Normal * 0.5f));

			sphere1->resolveCollsion(sphere2, 0.5f *(sphere1->getPosition() + sphere2->getPosition()));
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool PhysicsScene::sphere2OABB(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::OABB2Plane(PhysicsObject *obj1, PhysicsObject *obj2)
{
	oabb* box = dynamic_cast<oabb*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	//if we are successful then test for collsion
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getWidth());

		//which side is the centre of mass on?
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPosition() - planeOrigin, plane->getNormal());

		//check all four corners to see if we've hit the plane
		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
			{
				//get the position of the corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();

				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				//this is the total velocity of the point
				float velocityIntoPlane = glm::dot(box->getVelocity() + box->getRotation() *
					(-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				//if this corner is on the opposite side from the COM,
				//and moving further in, we need to resolve the collision
				if ((distFromPlane > 0 && comFromPlane < 0 && velocityIntoPlane > 0) ||
					(distFromPlane < 0 && comFromPlane > 0 && velocityIntoPlane < 0))
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}
		//we've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			//get the average collision velocity into the plane
			//(covers linear and rotational velocity of all corners involved)
			float collisionV = contactV / (float)numContacts;

			//get the acceleration required to stop (retitution = 0) or reverse
			//(restitution = 1) the average velocity into the plane
			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);

			//and the average position at which we'll apply the force
			//(corner of edge centre)
			glm::vec2 localContact = (contact / (float)numContacts) - box->getPosition();

			//this is the perpendicular distance we apply the force at relative to 
			//the COM, so Torque = F * r
			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));

			//work out the "effective mass" - this is a combination of moment of
			//inertia and mass, and tells us how much the contact point velocity
			//will change with the force we're applying
			float mass0 = 1.0f / (1.0f / box->getMass() + (r * r) / box->getMoment());
			//and apply the force
			box->applyForce(acceleration * mass0, localContact);

		}
	}

	return false;
}

bool PhysicsScene::OABB2sphere(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::OABB2OABB(PhysicsObject *, PhysicsObject *)
{
	return false;
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	m_actors.pop_back();
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;

	//Update physics at a fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;

		//Check for collisions (Idealy you'd want to have some sort of)
		checkForCollision();
	}
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}




