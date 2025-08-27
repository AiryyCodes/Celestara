#pragma once

#include <string>

class Item
{
public:
    Item(const std::string &name)
        : m_Name(name) {}

    void SetTextureLayer(int layer) { m_TextureLayer = layer; }
    int GetTextureLayer() const { return m_TextureLayer; }

private:
    friend class ItemRegistry;

    int m_Id;
    std::string m_Name;

    // Invalid by default
    int m_TextureLayer = -1;
};

struct ItemStack
{
    int Id = -1;
    int Quantity = 0;
};
