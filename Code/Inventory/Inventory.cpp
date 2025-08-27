#include "Inventory/Item.h"
#include "Inventory/Inventory.h"

ItemStack *Inventory::GetItem(int slot)
{
    if (slot > m_Slots.size())
        return nullptr;

    return &m_Slots[slot].Item;
}

void Inventory::SetItem(int slot, ItemStack &item)
{
    if (slot > m_Slots.size())
        return;

    m_Slots[slot].Item = item;
}
