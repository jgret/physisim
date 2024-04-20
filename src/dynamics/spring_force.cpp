#include "spring_force.h"

psim::SpringForce::SpringForce(RigidBody& a, RigidBody& b, float r, float ks, float kd) : ForceObject(), a(a), b(b), r(r), ks(ks), kd(kd)
{
}

void psim::SpringForce::applyForce()
{
    Vector3f l = a.getPos() - b.getPos();
    Vector3f ldot = a.getVel() - b.getVel();
    
    float l_mag = l.mag();

    Vector3f fa = -(ks * (l_mag - r) + kd * (ldot * l) / l_mag) * (l / l_mag);
    Vector3f fb = (-1.0f) * fa;

    a.applyForce(fa);
    b.applyForce(fb);
}
