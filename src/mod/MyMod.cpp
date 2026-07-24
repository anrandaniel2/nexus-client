#include "mod/MyMod.h"

#include <pl/ModMenu.hpp>
#include <filesystem>

namespace nexus {

MyMod &MyMod::instance() {
    static MyMod instance;
    return instance;
}

MyMod::MyMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool MyMod::load() {
    auto &self = getSelf();
    self.getLogger().debug("Loading...");

    std::error_code ec;
    std::filesystem::create_directories(self.getDataDir(), ec);
    if (ec) {
        self.getLogger().error("Failed to create data directory {}: {}",
                               self.getDataDir().string(), ec.message());
        return false;
    }
    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) {
        self.getLogger().error("Failed to create config directory {}: {}",
                               self.getConfigDir().string(), ec.message());
        return false;
    }

    mConfigFile.emplace();
    if (!mConfigFile->load()) {
        self.getLogger().warn("Failed to load typed config");
        return false;
    }
    mConfig = mConfigFile->value();

    self.getLogger().info("Loaded {} from {}", self.getName(), self.getModDir().string());
    return true;
}

bool MyMod::enable() {
    auto &self = getSelf();
    self.getLogger().debug("Enabling...");

    if (!mConfig.enabled) {
        self.getLogger().info("NexusClient is disabled by config");
        return true;
    }

    self.getLogger().info("Config message: {}", mConfig.message);

    // Register modules into LeviLauncher mod menu
    using namespace pl::modmenu;

    auto mid = std::string(self.getId());

    ModuleBuilder("killaura", "KillAura").modId(mid)
        .description("Automatically attacks nearby entities")
        .config("range", "Range", ConfigType::SliderFloat, "3.5", "1.0", "7.0")
        .config("cps", "CPS", ConfigType::SliderInt, "12", "1", "20")
        .config("players", "Target Players", ConfigType::Toggle, "true")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("KillAura {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("reach", "Reach").modId(mid)
        .description("Extends attack reach distance")
        .config("distance", "Distance", ConfigType::SliderFloat, "4.0", "3.0", "7.0")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("Reach {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("fly", "Fly").modId(mid)
        .description("Allows free flight")
        .config("speed", "Speed", ConfigType::SliderFloat, "2.0", "0.5", "10.0")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("Fly {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("speed", "Speed").modId(mid)
        .description("Increases movement speed")
        .config("multiplier", "Multiplier", ConfigType::SliderFloat, "1.5", "1.0", "5.0")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("Speed {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("sprint", "Sprint").modId(mid)
        .description("Automatically sprints")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("Sprint {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("nofall", "NoFall").modId(mid)
        .description("Prevents fall damage")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("NoFall {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("esp", "ESP").modId(mid)
        .description("Shows player info through walls")
        .config("tracers", "Tracers", ConfigType::Toggle, "false")
        .config("nametags", "Nametags", ConfigType::Toggle, "true")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("ESP {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("fullbright", "Fullbright").modId(mid)
        .description("Maximum brightness everywhere")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("Fullbright {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("scaffold", "Scaffold").modId(mid)
        .description("Auto-places blocks under you")
        .config("tower", "Tower Mode", ConfigType::Toggle, "true")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("Scaffold {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("xray", "XRay").modId(mid)
        .description("See ores through blocks")
        .config("opacity", "Block Opacity", ConfigType::SliderFloat, "0.3", "0.0", "1.0")
        .onToggle([&self](std::string_view, bool e) { self.getLogger().info("XRay {}", e?"ON":"OFF"); })
        .registerModule();

    self.getLogger().info("NexusClient enabled with modules registered!");
    return true;
}

bool MyMod::disable() {
    getSelf().getLogger().debug("Disabling...");
    return true;
}

bool MyMod::unload() {
    getSelf().getLogger().debug("Unloading...");
    mConfigFile.reset();
    return true;
}

} // namespace nexus
