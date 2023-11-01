#include "physisim.h"
#include <cassert>
#include "raymath.h"

using namespace psim;

System::System()
{
    
}

psim::System::~System()
{
    for (RigidBody* body : objects)
        delete body;
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

    if (idx < objects.size())
    {

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
    pObj1->getPos() -= move;
    pObj2->getPos() += move;

    //const float kNormalForce = 100;
    //if (depth > 0)
    //{
    //    // ! normal vetor always points from pObj1 to pObj2 !
    //    Vector3f normalForce{ 0 };
    //    normalForce = kNormalForce * depth * normal;
    //    pObj1->applyForce(-1 * normalForce * pObj1->getMass());
    //    pObj2->applyForce(normalForce * pObj2->getMass());
    //}

    //Vector3f &normal1 = normal;
    //Vector3f normal2 = -1 * normal;
    //Vector3f normalForce1 = (normal2 * pObj1->getForce()) * normal1;
    //Vector3f normalForce2 = (normal1 * pObj2->getForce()) * normal2;
    //pObj1->applyForce(normal1);
    //pObj2->applyForce(normal2);

}

void psim::System::calculateImpulse(RigidBody* pObj1, RigidBody* pObj2, Vector3f& point, Vector3f& normal)
{

    const float velThreshold = 0.0;

    float pBefore = (pObj1->getLinearMomentum() + pObj2->getLinearMomentum()).mag();

    //if (normal * (pObj1->getVel() - pObj2->getVel()) > 0.0f)
    //    return;

    normal = normal.normalize();

    // calculate the velocity along the distance vector
    psim::Vector3f velocityBefore1n = (pObj1->getVel() * normal) * normal;
    psim::Vector3f velocityBefore2n = (pObj2->getVel() * normal) * normal;

    // calculate the normal components to the velocity
    psim::Vector3f normalComponents1 = pObj1->getVel() - velocityBefore1n;
    psim::Vector3f normalComponents2 = pObj2->getVel() - velocityBefore2n;



	//float v1bn = velocityBefore1n.mag();
	//float v2bn = velocityBefore2n.mag();

    float v1bn;
    if (velocityBefore1n * normal > 0)
    {
        v1bn = velocityBefore1n.mag();
    }
    else
    {
        v1bn = -velocityBefore1n.mag();
    }

    float v2bn;
    if (velocityBefore2n * normal > 0)
    {
        v2bn = velocityBefore2n.mag();
    }
    else
    {
        v2bn = -velocityBefore2n.mag();
    }

    if (std::abs(v1bn) < velThreshold)
        v1bn = 0;
    if (std::abs(v2bn) < velThreshold)
        v2bn = 0;
	float m1 = pObj1->getMass();
	float m2 = pObj2->getMass();
    float p1 = m1 * v1bn;
    float p2 = m2 * v2bn;
	float e = std::fminf(pObj1->getRestitution(), pObj2->getRestitution());

	float v1an = ((p1 + p2) + ( m2 * e * (v2bn - v1bn))) / (m1 + m2);
	float v2an = ((p1 + p2) - ( m1 * e * (v2bn - v1bn))) / (m1 + m2);

    psim::Vector3f velocityAfter1n = normal * v1an;
    psim::Vector3f velocityAfter2n = normal * v2an;

	pObj1->getVel() = velocityAfter1n + normalComponents1;
	pObj2->getVel() = velocityAfter2n + normalComponents2;

    float pAfter = (pObj1->getLinearMomentum() + pObj2->getLinearMomentum()).mag();

    float pAbsDiff = std::abs(pBefore - pAfter);
    if (pAbsDiff > std::numeric_limits<float>::epsilon())
    {
        std::cout << "Momentum lost in collsion: " << pBefore << " -> " << pAfter << " (" << pAbsDiff << ")" << std::endl;
    }

}

void psim::System::step(float fElapsedTime)
{
    
    for (auto& o : objects)
        o->update(fElapsedTime);

}

StateVector psim::System::getStateVector()
{

    int idx = 0;
    StateVector out(RIGIDBODY_SIZE_IN_STATE_VECTOR * objects.size());
    for (auto body : objects)
    {
        idx = body->appendToStateVector(out, idx);
    }

    return out;
}

void psim::System::update(StateVector& y)
{

    int idx = 0;
    for (auto body : objects)
    {
        idx = body->updateFromStateVector(y, idx);
    }

}

RigidBody* psim::System::raycastSelect(Ray &ray)
{
    Vector3f p;
    return raycastSelect(ray, p);
}

RigidBody* psim::System::raycastSelect(Ray& ray, Vector3f& contactPoint)
{

    RigidBody* trackBody = nullptr;

    for (auto& body : objects)
    {

        if (body->getShapeType() == psim::SPHERE)
        {
            psim::Sphere& s = (psim::Sphere&)body->getShape();

            RayCollision result = GetRayCollisionSphere(ray, body->getPos(), s.getRadius());

            if (result.hit)
            {
                trackBody = body;
                contactPoint = Vector3f{ result.point };
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

StateVector psim::system_dydt(float t, StateVector& y)
{

    std::vector<RigidBody*>& objects = Simulation::system.getObjects();
    StateVector ydot(y.size());

    for (int idx = 0; idx < objects.size(); idx++)
    {
        int offset = idx * RIGIDBODY_SIZE_IN_STATE_VECTOR;
        RigidBody& body = *objects[idx];
        Vector3f& acc = body.getAcc();
        const Vector3f& torque = body.getTorque();
        float inertia = body.getInertia();

        // dx/dt
        ydot[offset + 0] = y[offset + 3];
        ydot[offset + 1] = y[offset + 4];
        ydot[offset + 2] = y[offset + 5];

        // dv/dt
        ydot[offset + 3] = acc.x;
        ydot[offset + 4] = acc.y;
        ydot[offset + 5] = acc.z;

        Quaternion rotation = 
        {
            y[offset + 6],
            y[offset + 7],
            y[offset + 8],
            y[offset + 9]
        };

        Quaternion omega = 
        {
            y[offset + 10],
            y[offset + 11],
            y[offset + 12],
            0
        };

        // qdot = 1/2 * w * q
        Quaternion qdot = QuaternionScale(QuaternionMultiply(omega, rotation), 0.5f);
        
        //qdot.z *= -1;

        ydot[offset + 6] = qdot.x;
        ydot[offset + 7] = qdot.y;
        ydot[offset + 8] = qdot.z;
        ydot[offset + 9] = qdot.w;

        // w = L / I
        ydot[offset + 10] = y[offset + 13] / inertia;
        ydot[offset + 11] = y[offset + 14] / inertia;
        ydot[offset + 12] = y[offset + 15] / inertia;

        ydot[offset + 13] = torque.x;
        ydot[offset + 14] = torque.y;
        ydot[offset + 15] = torque.z;
    }

    return ydot;
}
