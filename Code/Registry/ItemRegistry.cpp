#include "Registry/ItemRegistry.h"
#include "Inventory/Item.h"
#include "Logger.h"
#include "Memory.h"
#include "Renderer/Texture.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void ItemRegistry::Init()
{
    s_Textures = CreateRef<Texture3D>(16, 16);

    Register("test");
    Register("test2");
}

void ItemRegistry::Register(const std::string &id, Ref<Item> item)
{
    int layer = s_Textures->AddTexture("Assets/Textures/Items/" + id + ".png");
    item->SetTextureLayer(layer);
    item->m_Id = s_Items.size();

    s_Items[id] = item;
}

void ItemRegistry::Register(const std::string &id)
{
    std::ifstream file("Assets/Items/" + id + ".json");
    if (file.peek() == std::ifstream::traits_type::eof())
    {
        LOG_ERROR("Could not find item '{}'", id);
        return;
    }

    json entry = json::parse(file);

    ItemData data = ParseItemData(entry);

    Ref<Item> item = CreateRef<Item>(data);

    Register(data.Id, item);
}

ItemData ItemRegistry::ParseItemData(const json &entry)
{
    ItemData data;
    data.Name = entry.at("name").get<std::string>();
    data.Id = entry.at("id").get<std::string>();

    int maxStackSize = entry.value("max_stack_size", 100);
    if (maxStackSize < MIN_ALLOWED_STACK_SIZE)
        maxStackSize = MIN_ALLOWED_STACK_SIZE;
    if (maxStackSize > MAX_ALLOWED_STACK_SIZE)
        maxStackSize = MAX_ALLOWED_STACK_SIZE;
    data.MaxStackSize = maxStackSize;

    return data;
}

const Ref<Item> ItemRegistry::GetItem(const std::string &id)
{
    auto it = s_Items.find(id);
    return it == s_Items.end() ? nullptr : it->second;
}

const Ref<Item> ItemRegistry::GetItem(int index)
{
    if (index > s_Items.size())
        return nullptr;

    auto it = s_Items.begin();
    std::advance(it, index);
    return it->second;
}
