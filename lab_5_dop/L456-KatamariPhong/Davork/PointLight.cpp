#include "PointLight.h"

#include <iostream>

#include "../Davork/Game.h"


PointLight::PointLight(Game* g) : position_(0.0f, 1.0f, 0.0f),
lightColor_(1.0f, 1.0f, 1.0f, 1.0f), game_(g)
{
}