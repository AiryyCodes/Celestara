#pragma once

#include "Math/Math.h"

#include <cstdint>

enum class Direction : uint8_t
{
    None = 0,
    North,
    South,
    East,
    West,
};

static inline constexpr std::array<Direction, 4> DIRECTIONS = {
    Direction::North,
    Direction::South,
    Direction::East,
    Direction::West,
};

static inline Vector2i GetRelativeVector(Vector2i vector, Direction direction)
{
    switch (direction)
    {
    case Direction::North:
        return vector + Vector2i(0, 1);
    case Direction::South:
        return vector + Vector2i(0, -1);
    case Direction::West:
        return vector + Vector2i(-1, 0);
    case Direction::East:
        return vector + Vector2i(1, 0);
    default:
        return vector;
    }
}
