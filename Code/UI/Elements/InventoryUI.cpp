#include "UI/Elements/InventoryUI.h"
#include "Input.h"
#include "Inventory/Inventory.h"
#include "Inventory/Item.h"
#include "Memory.h"
#include "Registry/ItemRegistry.h"
#include "Math/Math.h"
#include "Renderer/Font.h"
#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include <string>

InventorySlotUI::InventorySlotUI(int row, int column, int index, const ItemStack &item)
    : UIElement(Vector2i(16, 16), "Assets/Textures/Transparent.png"),
      m_Row(row), m_Column(column), m_Index(index), m_Item(item)
{
    m_Mesh.SetVertices(UI_VERTICES);
}

void InventorySlotUI::Render()
{
    if (m_Item.IsEmpty())
        return;

    Ref<Item> item = m_Item.GetItem();

    Renderer::Begin(Renderer::GetSlotShader());
    m_Mesh.SetTexture(ItemRegistry::GetTextures());
    Renderer::GetSlotShader().SetUniform("u_Layer", item->GetTextureLayer());
    Renderer::SubmitUI(m_Mesh, GetPosition(), GetSize(), GetScale());

    std::string quantity = std::to_string(m_Item.GetQuantity());

    float textWidth = FontManager::GetTextWidth(Font::Main, quantity, 0.5f);

    // position text so its right edge lines up with the slot’s right edge
    float textX = GetPosition().x + GetSize().x * GetScale() - textWidth - 2.0f;
    float textY = GetPosition().y + 2.0f;

    Renderer::SubmitText(quantity, Font::Main, Vector2f(textX, textY), 0.5f, Vector3(1.0f, 1.0f, 1.0f));
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

    if (!m_CursorItem.IsEmpty())
    {
        const Ref<Item> item = m_CursorItem.GetItem();

        Renderer::Begin(Renderer::GetSlotShader());
        m_Mesh.SetTexture(ItemRegistry::GetTextures());
        Renderer::GetSlotShader().SetUniform("u_Layer", item->GetTextureLayer());

        double mouseX = Input::GetMouseX();
        double mouseY = Input::GetMouseY();

        Window *window = Renderer::GetMainWindow();
        int flippedY = window->GetHeight() - mouseY;

        // slot size in UI coords
        Vector2i slotSize(16, 16);
        float scale = GetScale();

        // position where the dragged slot is rendered
        Vector2f slotPos(mouseX - slotSize.x * scale / 2.0f,
                         flippedY - slotSize.y * scale / 2.0f);

        // draw the slot item
        Renderer::SubmitUI(m_Mesh, slotPos, slotSize, scale);

        // draw the quantity
        std::string quantity = std::to_string(m_CursorItem.GetQuantity());
        float textScale = 0.5f;

        float textWidth = FontManager::GetTextWidth(Font::Main, quantity, textScale);

        // bottom-right inside the slot bounds
        float textX = slotPos.x + slotSize.x * scale - textWidth - 2.0f;
        float textY = slotPos.y + 2.0f;

        Renderer::SubmitText(quantity, Font::Main, Vector2f(textX, textY),
                             textScale, Vector3(1.0f, 1.0f, 1.0f));
    }
}

void InventoryUI::OnClick(int x, int y, int button)
{
    Window *window = Renderer::GetMainWindow();

    int flippedY = window->GetHeight() - y;

    for (auto &slot : m_Slots)
    {
        bool inside = slot.IsInside(x, flippedY);
        if (inside)
        {
            HandleSlotClick(slot, button);
            return;
        }
    }
}

void InventoryUI::OnWindowResize(int width, int height)
{
    Center();

    FillSlots(m_Rows, m_Columns);
}

void InventoryUI::HandleSlotClick(InventorySlotUI &slot, int button)
{
    ItemStack &slotItem = slot.GetItem();

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (m_CursorItem.IsEmpty() && !slotItem.IsEmpty())
        {
            // Split the stack
            int total = slotItem.GetQuantity();
            int half = (total + 1) / 2; // cursor gets the larger half
            int remain = total - half;

            m_CursorItem = ItemStack(slotItem.GetItem(), half);
            if (remain > 0)
                slotItem.SetQuantity(remain);
            else
                slot.Clear();
        }
        else if (!m_CursorItem.IsEmpty())
        {
            // Place one item from cursor into slot
            if (slotItem.IsEmpty())
            {
                slot.SetItem(ItemStack(m_CursorItem.GetItem(), 1));
                m_CursorItem.AddQuantity(-1);
            }
            else if (slotItem.GetItem()->GetId() == m_CursorItem.GetItem()->GetId())
            {
                // Add one item if stack not full
                if (slotItem.GetQuantity() < slotItem.GetItem()->GetMaxStackSize())
                {
                    slotItem.AddQuantity(1);
                    m_CursorItem.AddQuantity(-1);
                }
            }

            if (m_CursorItem.GetQuantity() <= 0)
                m_CursorItem.Clear();
        }
        return;
    }

    // 1️⃣ Cursor is empty → pick up slot
    if (m_CursorItem.IsEmpty())
    {
        m_CursorItem = slotItem;
        slot.Clear();
        return;
    }

    // 2️⃣ Slot is empty → place cursor there
    if (slotItem.IsEmpty())
    {
        slot.SetItem(m_CursorItem);
        m_CursorItem.Clear();
        return;
    }

    slotItem.TryMerge(m_CursorItem);

    // If cursor still has items left after merging, swap them
    if (!m_CursorItem.IsEmpty())
    {
        std::swap(slotItem, m_CursorItem);
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

            InventorySlotUI uiSlot(row, col, index, *m_Inventory.GetItem(index));
            uiSlot.SetScale(slotScale);
            uiSlot.SetSize(slotSize);
            uiSlot.SetPosition({x, y});

            m_Slots.push_back(uiSlot);
        }
    }
}
