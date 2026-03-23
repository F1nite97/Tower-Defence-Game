#pragma once

#include "vector2.h"

class Particle
{
public:
    Vector2 m_position;
    Vector2 m_velocity;
    Vector2 m_acceleration;
    float m_age;
};