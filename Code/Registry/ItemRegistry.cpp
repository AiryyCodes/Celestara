#include "Registry/ItemRegistry.h"
#include "Memory.h"
#include "Renderer/Texture.h"

void ItemRegistry::Init()
{
    s_Textures = CreateRef<Texture3D>(16, 16);

    Register("test", CreateScope<Item>("Test"));
}

void ItemRegistry::Register(const std::string &id, Ref<Item> item)
{
    int layer = s_Textures->AddTexture("Assets/Textures/Items/" + id + ".png");
    item->SetTextureLayer(layer);
    item->m_Id = s_Items.size();

    s_Items[id] = item;
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
