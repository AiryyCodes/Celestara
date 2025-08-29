#pragma once

#include "Memory.h"

#include <string>

#define MIN_ALLOWED_STACK_SIZE 1
#define MAX_ALLOWED_STACK_SIZE 9999

struct ItemData
{
    std::string Id;
    std::string Name;
    int MaxStackSize = 100;
};

class Item
{
public:
    Item(const ItemData &data)
        : m_Name(data.Name), m_MaxStackSize(data.MaxStackSize) {}

    int GetId() const { return m_Id; }

    const std::string &GetName() const { return m_Name; }

    int GetMaxStackSize() const { return m_MaxStackSize; }

    int GetTextureLayer() const { return m_TextureLayer; }
    void SetTextureLayer(int layer) { m_TextureLayer = layer; }

private:
    friend class ItemRegistry;

    int m_Id;
    std::string m_Name;

    int m_MaxStackSize;

    // Invalid by default
    int m_TextureLayer = -1;
};

class ItemStack
{
public:
    ItemStack()
        : m_Item(nullptr), m_Quantity(0) {}

    ItemStack(const Ref<Item> &item, int quantity)
        : m_Item(item), m_Quantity(quantity) {}

    void Clear();

    int TryMerge(ItemStack &other);

    bool IsEmpty() const { return m_Item == nullptr || m_Quantity <= 0; }

    Ref<Item> GetItem() const { return m_Item; }

    int GetQuantity() const { return m_Quantity; }
    void SetQuantity(int quantity) { m_Quantity = quantity; }

private:
    Ref<Item> m_Item = nullptr;
    int m_Quantity = 0;
};
