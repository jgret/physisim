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
    float v1bn = pObj1->getVel() * normal;
    float v2bn = pObj2->getVel() * normal;
    float m1 = pObj1->getMass();
    float m2 = pObj2->getMass();
    float e = std::fminf(pObj1->getRestitution(), pObj2->getRestitution());

    float v1an = (m1 * v1bn + m2 * v2bn + m2 * e * (v2bn - v1bn)) / (m1 + m2);
    float v2an = (m1 * v1bn + m2 * v2bn - m2 * e * (v2bn - v1bn)) / (m1 + m2);

    pObj1->getVel() = normal * v1an;
    pObj2->getVel() = normal * v2an;
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
                body->setColor(GREEN);
                trackBody = body;
            }
            else
            {
                body->setColor(BLUE);
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
