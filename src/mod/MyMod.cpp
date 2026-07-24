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
        self.getLogger().info("nexus_client is disabled by config");
        return true;
    }

    self.getLogger().info("Config message: {}", mConfig.message);

    // Register modules — don't set modId, let ScopedModMenuOwner handle it
    using namespace pl::modmenu;

    {
        ModuleInfo info;
        info.moduleId = "killaura";
        info.displayName = "KillAura";
        info.description = "Automatically attacks nearby entities";
        info.defaultEnabled = false;
        info.configs.push_back({"range", "Range", ConfigType::SliderFloat, "3.5", "1.0", "7.0", ""});
        info.configs.push_back({"cps", "CPS", ConfigType::SliderInt, "12", "1", "20", ""});
        info.configs.push_back({"players", "Target Players", ConfigType::Toggle, "true", "", "", ""});
        bool ok = registerModule(info);
        self.getLogger().info("KillAura registered: {}", ok);
    }
    {
        ModuleInfo info;
        info.moduleId = "reach";
        info.displayName = "Reach";
        info.description = "Extends attack reach distance";
        info.configs.push_back({"distance", "Distance", ConfigType::SliderFloat, "4.0", "3.0", "7.0", ""});
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "fly";
        info.displayName = "Fly";
        info.description = "Allows free flight";
        info.configs.push_back({"speed", "Speed", ConfigType::SliderFloat, "2.0", "0.5", "10.0", ""});
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "speed";
        info.displayName = "Speed";
        info.description = "Increases movement speed";
        info.configs.push_back({"multiplier", "Multiplier", ConfigType::SliderFloat, "1.5", "1.0", "5.0", ""});
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "sprint";
        info.displayName = "Sprint";
        info.description = "Automatically sprints";
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "nofall";
        info.displayName = "NoFall";
        info.description = "Prevents fall damage";
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "esp";
        info.displayName = "ESP";
        info.description = "Shows player info through walls";
        info.configs.push_back({"tracers", "Tracers", ConfigType::Toggle, "false", "", "", ""});
        info.configs.push_back({"nametags", "Nametags", ConfigType::Toggle, "true", "", "", ""});
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "fullbright";
        info.displayName = "Fullbright";
        info.description = "Maximum brightness everywhere";
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "scaffold";
        info.displayName = "Scaffold";
        info.description = "Auto-places blocks under you";
        info.configs.push_back({"tower", "Tower Mode", ConfigType::Toggle, "true", "", "", ""});
        registerModule(info);
    }
    {
        ModuleInfo info;
        info.moduleId = "xray";
        info.displayName = "XRay";
        info.description = "See ores through blocks";
        info.configs.push_back({"opacity", "Block Opacity", ConfigType::SliderFloat, "0.3", "0.0", "1.0", ""});
        registerModule(info);
    }

    self.getLogger().info("NexusClient modules registered!");
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
