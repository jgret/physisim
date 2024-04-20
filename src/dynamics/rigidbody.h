#pragma once

#include "../common/vector3f.h"
#include "../collision/shape.h"
#include "../common/physic_constant.h"
#include "../solver/rk4solver.h"
#include "../common/matrix3x3.h"

namespace psim
{

#define RIGIDBODY_SIZE_IN_STATE_VECTOR 16

    class RigidBodyData
    {

    public:
        Vector3f pos;
        Vector3f vel;
        Vector3f force;

        Quaternion rotation;
        Vector3f omega;
        Vector3f torque;
        Vector3f angularMomentum;

        float mass;
        float inertia; // let's treat inertia as a scalar, because we only have spheres...
        Matrix3x3 inertiaTensor;
        float density;
        float restitution;
        float damping;

    };

    class RigidBody
    {

    private:
        static int nextId;

        int id;
        bool drawVectors;
        Shape* shape;

    protected:
        Color color;

        bool isStatic;
        RigidBodyData data;

    public:

        RigidBody(Shape* shape);

        RigidBody(const Vector3f& position, Shape* shape, const float density = 1, const float restitution = DEFAULT_RESTITIUTION, const Color& color = RED, bool drawVectors = false, bool isStatic = false);
        
        RigidBody(RigidBodyData data);

        ~RigidBody();

        /**
         * @brief called once to initialize object
        */
        virtual void init();

        /**
         * @brief update the objects physics
         * @param fElapsedTime
        */
        virtual void update(float fElapsedTime);

        /**
        * @brief draw the object on the screen
        */
        virtual void draw();

        /**
         * @brief clear all forces on object. sets acc to 0
        */
        void clearForces();

        /**
         * @brief apply force to the rigidbody's center of mass
         * @param force
        */
        void applyForce(const Vector3f& force);

        /**
         * @brief apply force to the rigidbody at the local point p,
         *		  relative to the center of mass.
         * @param force
         * @param point
        */
        void applyForce(const Vector3f& force, const Vector3f& p);

        /**
         * @brief add acceleration to the rigid body
         * @param acc
        */
        void addAcceleration(Vector3f acc);

        /**
         * @brief calculates the total energy of the object
         * @return
        */
        float getTotalEnergy();

        void showVectors(bool b);

        bool isShowVectors();

        float getMass();

        float getRestitution();

        float getDamping();

        void setDamping(float d);

        void setRestitution(const float r);

        Shape& getShape() const;

        ShapeType getShapeType() const;

        Vector3f& getPos();

        Vector3f& getVel();

        Vector3f getAcc();

        const Vector3f& getForce() const;

        const Vector3f& getTorque() const;

        Vector3f getLinearMomentum() const;

        const Vector3f& getAngularMomentum() const;

        const float getInertia() const;

        Matrix getTransform() const;

        Vector3f getVelAtPoint(const Vector3f& p) const;

        BoundingBox getAABB() const;

        void setColor(Color c);

        int getId();

        int appendToStateVector(StateVector& y, int idx);

        int updateFromStateVector(const StateVector& y, int idx);

        bool operator==(const RigidBody& other) const
        {
            return this->id == other.id;
        }
    };

    Vector3f getLocalPosition(const Vector3f& origin, const Vector3f& p);

    Vector3f getWorldPosition(const Vector3f& origin, const Vector3f lp);

}