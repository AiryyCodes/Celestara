#pragma once

#include "Memory.h"
#include "Renderer/Texture.h"
#include "World/Tile.h"
#include <string>
#include <unordered_map>

class TileRegistry
{
public:
    static void Init();

    static void Register(const std::string &id, Ref<Tile> tile);

    static const Texture3D &GetTextures() { return *s_Textures.get(); }

    static const Ref<Tile> GetTile(const std::string &id);

private:
    static inline std::unordered_map<std::string, Ref<Tile>> s_Tiles;

    static inline Ref<Texture3D> s_Textures;
};
