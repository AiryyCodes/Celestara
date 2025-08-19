#include "Registry/TileRegistry.h"
#include "Memory.h"
#include "World/Tile.h"

void TileRegistry::Init()
{
    s_Textures = CreateRef<Texture3D>(16, 16);

    Register("air", CreateScope<Tile>());
    Register("metal", CreateScope<Tile>());
}

void TileRegistry::Register(const std::string &id, Ref<Tile> tile)
{
    int layer = s_Textures->AddTexture("Assets/Textures/Tiles/" + id + ".png");
    tile->SetTextureLayer(layer);
    s_Tiles[id] = tile;
}

const Ref<Tile> TileRegistry::GetTile(const std::string &id)
{
    auto it = s_Tiles.find(id);
    return it == s_Tiles.end() ? nullptr : it->second;
}
