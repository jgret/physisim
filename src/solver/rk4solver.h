#pragma once
#include <valarray>

namespace psim
{
    typedef std::valarray<float> StateVector;
    
    class Rk4Solver
    {
    public: 


        /**
         * @brief this function computes the derivative of y
         * @param t simulation time
         * @param y state vector
         * @return derivative of state vector
        */
        typedef StateVector(*dydt_func) (float t, const StateVector& y);

        /**
         * @brief compute the new state of the state array (?)
         * @param y
         * @param y_out
         * @param t0
         * @param dt
         * @param func
        */
        void step(StateVector y, StateVector& y_out, float t0, float dt, dydt_func func);

    };

}
