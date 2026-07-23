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
    self.getLogger().info("NexusClient enabling...");
    self.getLogger().info("Mod ID: {}", self.getId());
    self.getLogger().info("Mod Name: {}", self.getName());

    // Test: register one simple module first
    bool testResult = pl::modmenu::ModuleBuilder("nexus_test", "NexusClient Test")
        .description("Test module to verify registration works")
        .modId(std::string(self.getId()))
        .defaultEnabled(false)
        .config("test_slider", "Test Slider", pl::modmenu::ConfigType::SliderFloat, "5.0", "0.0", "10.0")
        .config("test_toggle", "Test Toggle", pl::modmenu::ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) {
            getSelf().getLogger().info("Test module toggled: {}", e);
        })
        .registerModule();

    self.getLogger().info("Test module registered: {}", testResult ? "SUCCESS" : "FAILED");

    // Now register all real modules using the ACTUAL mod ID
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
