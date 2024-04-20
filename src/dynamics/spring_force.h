#pragma once
#include "force_object.h"
#include "rigidbody.h"

namespace psim
{

    class SpringForce : public ForceObject
    {

    private:
        RigidBody& a;
        RigidBody& b;
        float r; // rest length
        float ks; // spring constant
        float kd; // damping constant

    public:
        SpringForce(RigidBody& a, RigidBody& b, float r, float ks, float kd);

        virtual void applyForce();

    };

}
