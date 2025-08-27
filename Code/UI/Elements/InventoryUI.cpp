#include "UI/Elements/InventoryUI.h"
#include "Inventory/Inventory.h"
#include "Inventory/Item.h"
#include "Memory.h"
#include "Registry/ItemRegistry.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"

InventorySlotUI::InventorySlotUI(int row, int column, int index, ItemStack *item)
    : UIElement(Vector2i(16, 16), "Assets/Textures/Transparent.png"),
      m_Row(row), m_Column(column), m_Item(item)
{
    m_Mesh.SetVertices(UI_VERTICES);
}

void InventorySlotUI::Render()
{
    if (!m_Item)
        return;

    if (m_Item->Id == -1 || m_Item->Quantity <= 0)
        return;

    Ref<Item> item = ItemRegistry::GetItem(m_Item->Id);
    if (!item)
        return;

    Renderer::Begin(Renderer::GetSlotShader());
    m_Mesh.SetTexture(ItemRegistry::GetTextures());
    Renderer::GetSlotShader().SetUniform("u_Layer", item->GetTextureLayer());
    Renderer::SubmitUI(m_Mesh, GetPosition(), GetSize(), GetScale());
}

void InventorySlotUI::OnClick()
{
}

InventoryUI::InventoryUI(Inventory &inventory)
    : UIElement(Vector2i(75, 75), "Assets/Textures/UI/Inventory.png"),
      m_Inventory(inventory)
{
    SetScale(3);

    Center();
}

void InventoryUI::Render()
{
    for (InventorySlotUI &slot : m_Slots)
    {
        slot.Render();
    }
}

void InventoryUI::OnClick()
{
}

void InventoryUI::OnWindowResize(int width, int height)
{
    Center();

    FillSlots(m_Rows, m_Columns);
}

void InventoryUI::FillSlots(int rows, int columns)
{
    m_Rows = rows;
    m_Columns = columns;

    m_Slots.clear();
    m_Slots.reserve(rows * columns);

    // Inventory panel position & size
    Vector2f invPos = GetPosition(); // bottom-left of panel
    Vector2f invSize = GetSize();    // bottom-left of panel
    float invW = GetSize().x * GetScale();
    float invH = GetSize().y * GetScale();

    // Slot size
    Vector2i slotSize = {16, 16};
    int slotScale = 3;
    float slotW = slotSize.x * slotScale;
    float slotH = slotSize.y * slotScale;
    float spacing = 3.0f; // spacing between slots
    float padding = 4.0f; // optional padding from panel edges

    // Top-left corner for slots grid (inside panel)
    float startX = invPos.x + (padding * GetScale());
    float startY = invPos.y + (padding * GetScale());

    int index = 0;
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            InventorySlotUI uiSlot(row, col, index, m_Inventory.GetItem(index));
            uiSlot.SetScale(slotScale);
            uiSlot.SetSize(slotSize);

            float x = startX + col * (slotW + spacing);
            float y = startY + row * (slotH + spacing);

            uiSlot.SetPosition({x, y});
            m_Slots.push_back(uiSlot);

            index++;
        }
    }
}
