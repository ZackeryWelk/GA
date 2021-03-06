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

void PhysicsScene::addActor(PhysicsObject * actor)
{
	if (std::find(m_actors.begin(), m_actors.end(), actor) == m_actors.end())
	{
		m_actors.push_back(actor);
	}
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
		}
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

void PhysicsScene::update(float dt)
{
	checkForCollisions();
	static std::list<PhysicsObject*> dirty;
	// update the physics at a fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::updatedGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane,	PhysicsScene::plane2Sphere,  PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane,	PhysicsScene::box2Sphere,	 PhysicsScene::box2Box
};

void PhysicsScene::checkForCollisions()
{
	int actorCount = m_actors.size();

	//need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			//using function pointers
			int functionIdx = (shapeId1 * SHAPECOUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				//did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//isnt possible
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{	
	//Plane *plane = dynamic_cast<Plane*>(obj2);
	//Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	//
	////if this check is successful then test for collision
	//if (plane != nullptr && sphere != nullptr)
	//{
	//
	//
	//	glm::vec2 collisionNormal = plane->getNormal();
	//	float planeToSphere = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
	//
	//
	//	float intersection = sphere->getRadius() - planeToSphere;
	//	if (intersection > 0)
	//	{
	//		sphere->setPosition(sphere->getPosition() + collisionNormal * intersection);
	//
	//		plane->resolveCollision(sphere);
	//		return true;
	//	}
	//
	//}
	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//Plane *plane = dynamic_cast<Plane*>(obj1);
	//Square *box = dynamic_cast<Square*>(obj2);
	//
	//if (box != nullptr && plane != nullptr)
	//{
	//	box->SetLinearDrag(0.01);
	//	glm::vec2 collisionNormal = plane->getNormal();
	//
	//	float X = box->getExtents().x;
	//	float y = box->getExtents().y;
	//
	//	glm::vec2 topLeft(box->getPosition().x - box->getExtents().x / 4, box->getPosition().y + box->getExtents().y / 4);
	//	glm::vec2 topRight(box->getPosition().x + box->getExtents().x / 4, box->getPosition().y + box->getExtents().y / 4);
	//	glm::vec2 BottomLeft(box->getPosition().x - box->getExtents().x / 4, box->getPosition().y - box->getExtents().y / 4);
	//	glm::vec2 BottomRight(box->getPosition().x + box->getExtents().x / 4, box->getPosition().y - box->getExtents().y / 4);
	//
	//	float tLPlane = glm::dot(topLeft, plane->getNormal()) - plane->getDistance();
	//	float tRPlane = glm::dot(topRight, plane->getNormal()) - plane->getDistance();
	//	float bLPlane = glm::dot(BottomLeft, plane->getNormal()) - plane->getDistance();
	//	float bRPlane = glm::dot(BottomRight, plane->getNormal()) - plane->getDistance();
	//
	//	float intersectionRX = X - tRPlane;
	//	float intersectionRY = y - bRPlane;
	//	float intersectionLX = X - tLPlane;
	//	float intersectionLY = y - bLPlane;
	//
	//	if (intersectionRX > 0)
	//	{
	//		box->setPosition(box->getPosition() + collisionNormal * intersectionRX);
	//		plane->resolveCollision(box);
	//		box->SetLinearDrag(0.3);
	//		return true;
	//	}
	//	else if (intersectionRY > 0)
	//	{
	//		box->setPosition(box->getPosition() + collisionNormal * intersectionRY);
	//		plane->resolveCollision(box);
	//		box->SetLinearDrag(0.3);
	//		return true;
	//	}
	//	else if (intersectionLX > 0)
	//	{
	//		box->setPosition(box->getPosition() + collisionNormal * intersectionLX);
	//		plane->resolveCollision(box);
	//		box->SetLinearDrag(0.3);
	//		return true;
	//	}
	//	else if (intersectionLY > 0)
	//	{
	//		box->setPosition(box->getPosition() + collisionNormal * intersectionLY);
	//		plane->resolveCollision(box);
	//		box->SetLinearDrag(0.3);
	//		return true;
	//	}
	//}
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	//Plane *plane = dynamic_cast<Plane*>(obj2);
	//
	////if this check is successful then test for collision
	//if (sphere != nullptr && plane != nullptr)
	//{
	//
	//
	//	glm::vec2 collisionNormal = plane->getNormal();
	//	float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();
	//
	//
	//	float intersection = sphere->getRadius() - sphereToPlane;
	//	if (intersection > 0)
	//	{
	//		sphere->setPosition(sphere->getPosition() + collisionNormal * intersection);
	//
	//		plane->resolveCollision(sphere);
	//		return true;
	//	}
	//
	//}
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//Sphere *sphere1 = dynamic_cast<Sphere*>(obj1);
	//Sphere *sphere2 = dynamic_cast<Sphere*>(obj2);
	////if it is successful then test for collision
	//if (sphere1 != nullptr && sphere2 != nullptr)
	//{
	//	glm::vec2 displacement = sphere1->getPosition() - sphere2->getPosition();
	//	if (glm::length(displacement) < (sphere1->getRadius() + sphere2->getRadius()))
	//	{
	//
	//		float overlap = (sphere1->getRadius() + sphere2->getRadius()) - (glm::length(displacement));
	//		glm::vec2 offset = glm::normalize(displacement) * overlap;
	//
	//
	//		sphere1->setPosition(sphere1->getPosition() + offset * 0.5f);
	//		sphere2->setPosition(sphere2->getPosition() + -offset * 0.5f);
	//
	//		sphere1->resolveCollision(sphere2);
	//
	//		return true;
	//	}
	//}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//Sphere *sphere = dynamic_cast<Sphere*>(obj1);
	//Square *box = dynamic_cast<Square*>(obj2);
	//
	//if (sphere != nullptr && box != nullptr)
	//{
	//
	//
	//	glm::vec2 topRight(box->getPosition().x + box->getExtents().x, box->getPosition().y + box->getExtents().y);
	//	glm::vec2 BottomLeft(box->getPosition().x - box->getExtents().x, box->getPosition().y - box->getExtents().y);
	//
	//	glm::vec2 closestPoint = glm::clamp(sphere->getPosition(), BottomLeft, topRight);
	//
	//	glm::vec2 displacement = sphere->getPosition() - closestPoint;
	//
	//	float overlap = sphere->getRadius() - glm::length(displacement);
	//
	//	if (overlap > 0)
	//	{
	//		glm::vec2 offset = glm::normalize(displacement) * overlap;
	//
	//		sphere->setPosition(sphere->getPosition() + offset * 0.5f);
	//		box->setPosition(box->getPosition() - offset * 0.5f);
	//		sphere->resolveCollision(box);
	//	}
	//}
	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{

	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	//Square *box1 = dynamic_cast<Square*>(obj1);
	//Square *box2 = dynamic_cast<Square*>(obj2);
	//
	//
	//if (box1 != nullptr && box2 != nullptr)
	//{
	//	glm::vec2 max1(box1->getPosition().x + box1->getExtents().x, box1->getPosition().y + box1->getExtents().y);
	//	glm::vec2 min1(box1->getPosition().x - box1->getExtents().x, box1->getPosition().y - box1->getExtents().y);
	//
	//	glm::vec2 max2(box2->getPosition().x + box2->getExtents().x, box2->getPosition().y + box2->getExtents().y);
	//	glm::vec2 min2(box2->getPosition().x - box2->getExtents().x, box2->getPosition().y - box2->getExtents().y);
	//
	//
	//	glm::vec2 closestPoint1 = glm::clamp(box1->getPosition(), min2, max2);
	//	glm::vec2 closestPoint2 = glm::clamp(box2->getPosition(), min1, max1);
	//
	//	glm::vec2 displacement = box1->getPosition() - closestPoint1;
	//
	//	float overlap = glm::distance(box1->getPosition(), closestPoint2) - glm::length(displacement);
	//
	//	glm::vec2 offset = glm::normalize(displacement) * overlap;
	//
	//	//aie::Gizmos::add2DLine(closestPoint2, box1->getPosition(),glm::vec4(1,1,1,1));
	//
	//	if (max1.x < min2.x || max1.y < min2.y ||
	//		min1.x > max2.x || min1.y > max2.y)
	//	{
	//		return false;
	//	}
	//	else
	//	{
	//		box1->setPosition(box1->getPosition() + offset * 0.3f);
	//		box2->setPosition(box2->getPosition() - offset * 0.3f);
	//
	//		box1->resolveCollision(box2);
	//		return true;
	//	}
	//
	//}
	return false;
}
