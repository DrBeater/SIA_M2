#include "integration.h"
#include <iostream>
using namespace std;

void explicitEulerStep (ODESystem *system, double dt) {
    // TODO: implement this
    VectorXd state(system->getDimensions());
    system->getState(state);

    VectorXd derivatives(system->getDimensions());
    system->getDerivative(derivatives);

    VectorXd newState(system->getDimensions());
    newState = state + dt * derivatives;
    system->setState(newState);

}

void midPointStep (ODESystem *system, double dt) {
    // TODO: implement this
    VectorXd state(system->getDimensions());
    system->getState(state);

    VectorXd derivatives(system->getDimensions());
    system->getDerivative(derivatives);

    VectorXd midState(system->getDimensions());
    midState = state + dt/2.f * derivatives;
    system->setState(midState);

    VectorXd newState(system->getDimensions());
    newState = state + dt * derivatives;
    system->setState(newState);
}
