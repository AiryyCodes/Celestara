#pragma once

#include "Inventory/Item.h"

#include <vector>

struct InventorySlot
{
    ItemStack Item;
};

class Inventory
{
public:
    Inventory(int rows, int columns)
        : m_Rows(rows), m_Columns(columns)
    {
        m_Slots.resize(columns * rows);
    }

    ItemStack *GetItem(int slot);
    void SetItem(int slot, const ItemStack &item);
    bool AddItem(const ItemStack &itemToAdd);

private:
    int m_Rows;
    int m_Columns;

    std::vector<InventorySlot> m_Slots;
};
