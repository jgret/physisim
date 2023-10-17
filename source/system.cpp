#include "physisim.h"

using namespace psim;

System::System()
{
    
}

void System::addRigidBody(psim::RigidBody* b)
{
    objects.push_back(b);
}

psim::RigidBody* System::removeRigidBody(psim::RigidBody* b)
{
    return removeRigidBodyById(b->getId());
}

psim::RigidBody* System::removeRigidBodyById(int search)
{
    int idx = 0;
    psim::RigidBody* ret = nullptr;
    
    // search for index
    while ((objects[idx]->getId() != search) && (idx < objects.size())) 
    {
        idx++;
    }

    if (idx < objects.size()) {

        // store address
        ret = objects.at(idx);

        // remove from vector
        auto it = objects.begin();
        std::advance(it, idx);
        objects.erase(it);

    }

    return ret;
}

void psim::System::clearForces()
{
    for (auto& o : objects)
    {
        o->clearForces();
    }
}

void System::applyForce(psim::Vector3f f)
{
    for (auto& o : objects)
    {
        o->applyForce(f);
    }

}

void psim::System::applyGravity()
{
    for (auto& o : objects)
    {
        o->addAcceleration(GRAVITY);
    }
}

void psim::System::checkCollision()
{

    for (int i = 0; i < objects.size(); i++)
    {
        for (int j = i + 1; j < objects.size(); j++)
        {

            RigidBody* pObj1 = objects[i];
            RigidBody* pObj2 = objects[j];

            Vector3f normal;
            Vector3f point;
            float depth;

            if (psim::Collision::checkCollision(*pObj1, *pObj2, point, normal, depth))
            {
                resolveCollision(pObj1, pObj2, normal, depth);
                calculateImpulse(pObj1, pObj2, point, normal);
            }

        }
    }


}

void psim::System::resolveCollision(RigidBody* pObj1, RigidBody* pObj2, Vector3f& normal, float depth)
{
    Vector3f move = normal * depth / 2;

    pObj1->getPos() += move;
    pObj2->getPos() -= move;
}

void psim::System::calculateImpulse(RigidBody* pObj1, RigidBody* pObj2, Vector3f& point, Vector3f& normal)
{

	if (normal * (pObj1->getVel() - pObj2->getVel()) > 0.0f)
		return;

    normal = normal.normalize();

    // calculate the velocity along the distance vector
    psim::Vector3f velocityBefore1n = normal * pObj1->getVel().cross(normal).mag();
    psim::Vector3f velocityBefore2n = normal * pObj2->getVel().cross(normal).mag();

    // calculate the normal components to the velocity
    psim::Vector3f normalComponents1 = pObj1->getVel() - velocityBefore1n;
    psim::Vector3f normalComponents2 = pObj1->getVel() - velocityBefore2n;

	float v1bn = velocityBefore1n.mag();
	float v2bn = velocityBefore2n.mag();
	float m1 = pObj1->getMass();
	float m2 = pObj2->getMass();
	float e = std::fminf(pObj1->getRestitution(), pObj2->getRestitution());

	float v1an = (m1 * v1bn + m2 * v2bn + m2 * e * (v2bn - v1bn)) / (m1 + m2);
	float v2an = (m1 * v1bn + m2 * v2bn - m2 * e * (v2bn - v1bn)) / (m1 + m2);

    psim::Vector3f velocityAfter1n = normal * v1an;
    psim::Vector3f velocityAfter2n = normal * v2an;

	pObj1->getVel() = velocityAfter1n + normalComponents1;
	pObj2->getVel() = velocityAfter2n + normalComponents2;

 //   Vector3f relativeVel = pObj2->getVel() - pObj1->getVel();

 //   if (relativeVel * normal > 0.0f)
 //       return;

	//float e = std::fminf(pObj1->getRestitution(), pObj2->getRestitution());

 //   float j = -(1.0f + e) * (relativeVel * normal);
 //   j /= (pObj1->getMass() + pObj2->getMass());

 //   Vector3f impulse = normal * j;


 //   pObj1->getVel() -= impulse * pObj1->getMass();
 //   pObj2->getVel() += impulse * pObj2->getMass();

}

void psim::System::step(float fElapsedTime)
{
    
    for (auto& o : objects)
        o->update(fElapsedTime);

}

RigidBody* psim::System::raycastSelect(Ray &ray)
{

    RigidBody* trackBody = nullptr;

    for (auto& body : objects)
    {

        if (body->getShapeType() == psim::SPHERE)
        {
            psim::Sphere& s = (psim::Sphere&)body->getShape();

            RayCollision result = GetRayCollisionSphere(ray, s.getPos(), s.getRadius());

            if (result.hit)
            {
                trackBody = body;
            }
        }
    }
    return trackBody;
}

float System::getTotalEnergy()
{
    float totalEnergy = 0;

    for (auto& a : objects)
        totalEnergy += a->getTotalEnergy();

    return totalEnergy;
}

std::vector<psim::RigidBody*>& System::getObjects()
{
    return objects;
}
