#include "UI/Elements/InventoryUI.h"
#include "Input.h"
#include "Inventory/Inventory.h"
#include "Inventory/Item.h"
#include "Memory.h"
#include "Registry/ItemRegistry.h"
#include "Math/Math.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

InventorySlotUI::InventorySlotUI(int row, int column, int index, ItemStack *item)
    : UIElement(Vector2i(16, 16), "Assets/Textures/Transparent.png"),
      m_Row(row), m_Column(column), m_Index(index), m_Item(item)
{
    m_Mesh.SetVertices(UI_VERTICES);
}

void InventorySlotUI::Render()
{
    if (!m_Item)
        return;

    if (m_Item->Id < 0 || m_Item->Quantity <= 0)
        return;

    Ref<Item> item = ItemRegistry::GetItem(m_Item->Id);
    if (!item)
        return;

    Renderer::Begin(Renderer::GetSlotShader());
    m_Mesh.SetTexture(ItemRegistry::GetTextures());
    Renderer::GetSlotShader().SetUniform("u_Layer", item->GetTextureLayer());
    Renderer::SubmitUI(m_Mesh, GetPosition(), GetSize(), GetScale());
}

InventoryUI::InventoryUI(Inventory &inventory)
    : UIElement(Vector2i(75, 75), "Assets/Textures/UI/Inventory.png"),
      m_Inventory(inventory)
{
    m_Mesh.SetVertices(UI_VERTICES);

    SetScale(3);

    Center();
}

void InventoryUI::Render()
{
    for (InventorySlotUI &slot : m_Slots)
    {
        slot.Render();
    }

    if (m_CursorItem)
    {
        // TODO: Migrate this to a "RenderItem" function
        const Ref<Item> item = ItemRegistry::GetItem(m_CursorItem->Id);
        if (!item)
            return;

        Renderer::Begin(Renderer::GetSlotShader());
        m_Mesh.SetTexture(ItemRegistry::GetTextures());
        Renderer::GetSlotShader().SetUniform("u_Layer", item->GetTextureLayer());

        double mouseX = Input::GetMouseX();
        double mouseY = Input::GetMouseY();

        Window *window = Renderer::GetMainWindow();

        int flippedY = window->GetHeight() - mouseY;

        Renderer::SubmitUI(m_Mesh, Vector2f(mouseX, flippedY) - Vector2f(16 * 3 / 2, 16 * 3 / 2), Vector2i(16, 16), GetScale());
    }
}

void InventoryUI::OnClick(int x, int y)
{
    Window *window = Renderer::GetMainWindow();

    int flippedY = window->GetHeight() - y;

    for (auto &slot : m_Slots)
    {
        bool inside = slot.IsInside(x, flippedY);
        if (inside)
        {
            HandleSlotClick(slot);
            return;
        }
    }
}

void InventoryUI::OnWindowResize(int width, int height)
{
    Center();

    FillSlots(m_Rows, m_Columns);
}

void InventoryUI::HandleSlotClick(InventorySlotUI &slot)
{
    ItemStack *slotItem = slot.GetItem();
    if (m_CursorItem == nullptr)
    {
        // Pick up from slot
        m_CursorItem = slotItem;
        slot.Clear();
    }
    else
    {
        ItemStack *temp = slotItem;
        slot.SetItem(m_CursorItem);
        m_CursorItem = temp;
    }
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

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < columns; col++)
        {
            int index = row * columns + col;

            float x = startX + col * (slotW + spacing);

            // Flip Y so row 0 is top
            float y = startY + (rows - 1 - row) * (slotH + spacing);

            InventorySlotUI uiSlot(row, col, index, m_Inventory.GetItem(index));
            uiSlot.SetScale(slotScale);
            uiSlot.SetSize(slotSize);
            uiSlot.SetPosition({x, y});

            m_Slots.push_back(uiSlot);
        }
    }
}
