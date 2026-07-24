#include "mod/MyMod.h"

#include <pl/ModMenu.hpp>
#include <filesystem>
#include <thread>

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
        self.getLogger().info("nexus_client is disabled by config");
        return true;
    }

    self.getLogger().info("Config message: {}", mConfig.message);

    // Register modules after a short delay so enable() returns first
    std::thread([&self]() {
        // Wait for enable lifecycle to complete
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        using namespace pl::modmenu;

        auto reg = [&self](const char* id, const char* name, const char* desc,
                           std::vector<ConfigEntry> configs = {}) {
            ModuleInfo info;
            info.moduleId = id;
            info.displayName = name;
            info.description = desc;
            info.modId = std::string(self.getId());
            info.configs = std::move(configs);
            bool ok = registerModule(info);
            self.getLogger().info("{} registered: {}", name, ok);
        };

        reg("killaura", "KillAura", "Auto-attacks nearby entities", {
            {"range", "Range", ConfigType::SliderFloat, "3.5", "1.0", "7.0", ""},
            {"cps", "CPS", ConfigType::SliderInt, "12", "1", "20", ""},
            {"players", "Target Players", ConfigType::Toggle, "true", "", "", ""},
        });
        reg("reach", "Reach", "Extends attack reach", {
            {"distance", "Distance", ConfigType::SliderFloat, "4.0", "3.0", "7.0", ""},
        });
        reg("fly", "Fly", "Allows free flight", {
            {"speed", "Speed", ConfigType::SliderFloat, "2.0", "0.5", "10.0", ""},
        });
        reg("speed", "Speed", "Increases movement speed", {
            {"multiplier", "Multiplier", ConfigType::SliderFloat, "1.5", "1.0", "5.0", ""},
        });
        reg("sprint", "Sprint", "Automatically sprints");
        reg("nofall", "NoFall", "Prevents fall damage");
        reg("esp", "ESP", "Shows player info through walls", {
            {"tracers", "Tracers", ConfigType::Toggle, "false", "", "", ""},
            {"nametags", "Nametags", ConfigType::Toggle, "true", "", "", ""},
        });
        reg("fullbright", "Fullbright", "Maximum brightness");
        reg("scaffold", "Scaffold", "Auto-places blocks under you", {
            {"tower", "Tower Mode", ConfigType::Toggle, "true", "", "", ""},
        });
        reg("xray", "XRay", "See ores through blocks", {
            {"opacity", "Opacity", ConfigType::SliderFloat, "0.3", "0.0", "1.0", ""},
        });

        self.getLogger().info("NexusClient: all modules registered!");
    }).detach();

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
