#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>
#include <filesystem>

namespace nexus {

NexusMod& NexusMod::instance() {
    static NexusMod inst;
    return inst;
}

NexusMod::NexusMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool NexusMod::load() {
    auto& self = getSelf();
    self.getLogger().info("NexusClient v1.0.0 loading...");

    std::error_code ec;
    std::filesystem::create_directories(self.getDataDir(), ec);
    std::filesystem::create_directories(self.getConfigDir(), ec);

    self.getLogger().info("NexusClient loaded from {}", self.getModDir().string());
    return true;
}

bool NexusMod::enable() {
    auto& self = getSelf();
    self.getLogger().info("NexusClient enabling — registering {} modules...", "30+");

    // Register all modules into LeviLauncher's mod menu
    registerCombatModules();
    registerMovementModules();
    registerRenderModules();
    registerPlayerModules();
    registerWorldModules();
    registerMiscModules();

    self.getLogger().info("NexusClient enabled!");
    return true;
}

bool NexusMod::disable() {
    auto& self = getSelf();
    self.getLogger().info("NexusClient disabling...");

    // Unregister all modules
    const char* ids[] = {
        "killaura","reach","criticals","autoclicker","antiknockback","velocity",
        "fly","speed","sprint","nofall","jesus","step","longjump","spider","glide",
        "esp","fullbright","nohurtcam","freecam","nametags","zoom",
        "autotool","fastplace","scaffold","nuker","noslow","cheststealer",
        "xray","timer","weather",
        "disabler","spammer","selfdestruct",
    };
    for (auto id : ids) {
        pl::modmenu::unregisterModule(id);
    }

    return true;
}

bool NexusMod::unload() {
    getSelf().getLogger().info("NexusClient unloading...");
    return true;
}

} // namespace nexus
