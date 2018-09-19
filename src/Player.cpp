
#include "Player.h"

Player::Player()
    : PerspectiveCamera(9.0f / 16.0f, 90.0f, 0.1f, 100.0f),
      accumulatedInput(Vec3f::Zero())
{
}
void Player::addMovementInput(Vec3f direction, float axis)
{
    accumulatedInput += direction.normalized() * axis;
}

void Player::applyMovement(double deltaTime)
{
    translate(accumulatedInput * deltaTime);
    accumulatedInput = Vec3f::Zero();
    updateViewMatrix();
}