#include "Inventory/Item.h"
#include "Inventory/Inventory.h"

ItemStack *Inventory::GetItem(int slot)
{
    if (slot > m_Slots.size())
        return nullptr;

    return &m_Slots[slot].Item;
}

void Inventory::SetItem(int slot, const ItemStack &item)
{
    if (slot > m_Slots.size())
        return;

    m_Slots[slot].Item = item;
}

bool Inventory::AddItem(const ItemStack &itemToAdd)
{
    int remaining = itemToAdd.GetQuantity();

    // Try to merge with existing stacks
    for (auto &slot : m_Slots)
    {
        if (!slot.Item.IsEmpty() && slot.Item.GetItem() == itemToAdd.GetItem())
        {
            int maxStack = itemToAdd.GetItem()->GetMaxStackSize();
            int space = maxStack - slot.Item.GetQuantity();

            if (space > 0)
            {
                int add = std::min(space, remaining);
                slot.Item.SetQuantity(slot.Item.GetQuantity() + add);
                remaining -= add;
                if (remaining == 0)
                    return true; // fully added
            }
        }
    }

    // Place in empty slots
    for (auto &slot : m_Slots)
    {
        if (slot.Item.IsEmpty())
        {
            int maxStack = itemToAdd.GetItem()->GetMaxStackSize();
            int add = std::min(maxStack, remaining);
            slot.Item = ItemStack(itemToAdd.GetItem(), add);
            remaining -= add;
            if (remaining == 0)
                return true; // fully added
        }
    }

    return false; // couldn't fit all items
}
