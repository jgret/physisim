#pragma once

#include <valarray>
#include <functional>

namespace psim
{
    typedef std::valarray<float> StateVector;

    /**
        * @brief this function computes the derivative of y
        * @param t simulation time
        * @param y state vector
        * @return derivative of state vector
    */
    typedef std::function<StateVector(float, const StateVector&)> dydt_func;

    class ODESolver
    {
    protected:
        dydt_func func;

    public:

        ODESolver(dydt_func f);

        /**
         * @brief compute the new state of the state array (?)
         * @param y
         * @param y_out
         * @param t0
         * @param dt
         * @param func
        */
        virtual void step(StateVector y, StateVector& y_out, float t0, float dt) = 0;
    };

}
