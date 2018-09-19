
#pragma once

#include "Camera.h"
class Player : public PerspectiveCamera
{
public:
    Player();

    void addMovementInput(Vec3f direction, float axis);

    void applyMovement(double deltaTime);

private:
    float xSens = 0.01f;
    float ySens = 0.01f;

    Vec3f accumulatedInput;
};