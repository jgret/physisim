#include "rk4solver.h"
#include <iostream>
#include <vector>

using namespace psim;

void Rk4Solver::step(StateVector& y, StateVector& out, float t0, float dt, dydt_func func)
{

    float dt_2 = dt / 2.0f;

    StateVector k1 = func(t0, y);
    StateVector k2 = func(t0 + dt_2, y + (dt_2 * k1));
    StateVector k3 = func(t0 + dt_2, y + (dt_2 * k2));
    StateVector k4 = func(t0 + dt, y + (dt * k3));

    out = y + dt * 1.0f / 6.0f * (k1 + 2 * k2 + 2 * k3 + k4);

}
