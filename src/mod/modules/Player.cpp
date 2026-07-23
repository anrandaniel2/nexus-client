#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

using namespace pl::modmenu;

namespace nexus {

void NexusMod::registerPlayerModules() {
    ModuleBuilder("autotool", "AutoTool")
        .description("Selects the best tool automatically").modId("nexus_client")
        .config("sword_attack", "Sword on Attack", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("AutoTool {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("fastplace", "FastPlace")
        .description("Place blocks faster").modId("nexus_client")
        .config("delay", "Delay (ticks)", ConfigType::SliderInt, "0", "0", "4")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("FastPlace {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("scaffold", "Scaffold")
        .description("Auto-places blocks under you").modId("nexus_client")
        .config("tower", "Tower Mode", ConfigType::Toggle, "true")
        .config("safewalk", "SafeWalk", ConfigType::Toggle, "true")
        .config("extend", "Extend", ConfigType::SliderFloat, "0", "0", "5")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Scaffold {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("nuker", "Nuker")
        .description("Break blocks in a radius").modId("nexus_client")
        .config("radius", "Radius", ConfigType::SliderFloat, "3.0", "1.0", "8.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Nuker {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("noslow", "NoSlow")
        .description("Removes item usage slowdown").modId("nexus_client")
        .config("multiplier", "Speed Multiplier", ConfigType::SliderFloat, "1.0", "0.5", "1.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("NoSlow {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("cheststealer", "ChestStealer")
        .description("Auto-takes items from chests").modId("nexus_client")
        .config("delay", "Delay (ms)", ConfigType::SliderInt, "50", "0", "500")
        .config("close_after", "Close After", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("ChestStealer {}", e?"ON":"OFF"); })
        .registerModule();
}

} // namespace nexus
