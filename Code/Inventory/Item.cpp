#include "Inventory/Item.h"

void ItemStack::Clear()
{
    m_Item = nullptr;
    m_Quantity = 0;
}

// Returns how many items were merged and reduces `other` quantity
int ItemStack::TryMerge(ItemStack &other)
{
    if (IsEmpty() || other.IsEmpty())
        return 0;
    if (m_Item->GetId() != other.m_Item->GetId())
        return 0;

    int maxStack = m_Item->GetMaxStackSize();
    int spaceLeft = maxStack - m_Quantity;
    int mergeAmount = std::min(spaceLeft, other.m_Quantity);

    m_Quantity += mergeAmount;
    other.m_Quantity -= mergeAmount;

    return mergeAmount;
}
