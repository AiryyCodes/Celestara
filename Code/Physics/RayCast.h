#pragma once

#include "Math/Math.h"
#include <box2d/id.h>
#include <box2d/math_functions.h>
#include <box2d/types.h>

struct RayCastContext
{
    b2ShapeId shapeId;
    b2Vec2 point;
    b2Vec2 normal;
    float fraction;
};

RayCastContext CastRay(Vector2f origin, Vector2f end, b2QueryFilter filter);

float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void *context);
