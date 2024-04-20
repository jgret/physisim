#pragma once
#include <valarray>
#include "odesolver.h"

namespace psim
{
    
    class Rk4Solver : public ODESolver
    {
    
    public:

        Rk4Solver(dydt_func f);

        virtual void step(StateVector y, StateVector& y_out, float t0, float dt);

    };

}
