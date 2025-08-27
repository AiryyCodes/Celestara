#pragma once

#include "Inventory/Inventory.h"
#include "Inventory/Item.h"
#include "Renderer/Mesh.h"
#include "UI/Element.h"

#include <vector>

class InventorySlotUI : public UIElement
{
public:
    InventorySlotUI(int row, int column, int index, ItemStack *item);

    void Render() override;

    void OnClick() override;

private:
    int m_Row;
    int m_Column;
    int m_Index;

    Mesh m_Mesh;

    ItemStack *m_Item;
};

class InventoryUI : public UIElement
{
public:
    InventoryUI(Inventory &inventory);

    void Render() override;

    void OnClick() override;
    void OnWindowResize(int width, int height) override;

    void FillSlots(int rows, int columns);

private:
    int m_Rows;
    int m_Columns;

    std::vector<InventorySlotUI> m_Slots;

    Inventory &m_Inventory;
};
