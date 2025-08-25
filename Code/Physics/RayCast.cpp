#include "Physics/RayCast.h"
#include "Game.h"
#include "Math/Math.h"
#include "Memory.h"
#include "World/World.h"

RayCastContext CastRay(Vector2f origin, Vector2f end, b2QueryFilter filter)
{
    Ref<World> world = Game::Get().GetWorld();

    RayCastContext context = {0};
    Vector2f translation = end - origin;
    b2World_CastRay(world->GetPhysicsId(), {origin.x, origin.y}, {translation.x, translation.y}, filter, RayCastCallback, &context);

    return context;
}

float RayCastCallback(b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void *context)
{
    RayCastContext *myContext = static_cast<RayCastContext *>(context);
    myContext->shapeId = shapeId;
    myContext->point = point;
    myContext->normal = normal;
    myContext->fraction = fraction;
    return fraction;
}
