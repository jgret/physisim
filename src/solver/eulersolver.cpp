#include "eulersolver.h"

psim::EulerSolver::EulerSolver(dydt_func f) : ODESolver(f)
{
}

void psim::EulerSolver::step(StateVector y, StateVector& out, float t0, float dt)
{
    out = y + dt * func(t0, y);
}
