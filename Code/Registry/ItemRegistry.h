#include "Inventory/Item.h"
#include "Memory.h"
#include "Renderer/Texture.h"

#include <nlohmann/json_fwd.hpp>
#include <unordered_map>

class ItemRegistry
{
public:
    static void Init();

    static void Register(const std::string &id, Ref<Item> item);
    static void Register(const std::string &id);

    static const Texture3D &GetTextures() { return *s_Textures.get(); }

    static const Ref<Item> GetItem(const std::string &id);
    static const Ref<Item> GetItem(int id);

private:
    static ItemData ParseItemData(const nlohmann::json &entry);

private:
    static inline std::unordered_map<std::string, Ref<Item>> s_Items;

    static inline Ref<Texture3D> s_Textures;
};
