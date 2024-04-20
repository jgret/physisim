#pragma once
#include "odesolver.h"

namespace psim
{

    class EulerSolver : public ODESolver
    {

    public:

        EulerSolver(dydt_func f);

        virtual void step(StateVector y, StateVector& y_out, float t0, float dt);

    };

}
