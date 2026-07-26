#include "mod/MyMod.h"

#include <pl/ModMenu.hpp>
#include <filesystem>

namespace nexus {

MyMod &MyMod::instance() {
    static MyMod instance;
    return instance;
}

bool MyMod::load(pl::mod::ModContext &context) {
    std::error_code ec;
    std::filesystem::create_directories(context.dataDir(), ec);
    std::filesystem::create_directories(context.configDir(), ec);
    context.logger().info("Loaded {}", context.name());
    return true;
}

bool MyMod::enable(pl::mod::ModContext &context) {
    using namespace pl::modmenu;

    auto reg = [&](const char* id, const char* name, const char* desc, std::vector<ConfigEntry> configs = {}) {
        ModuleInfo info;
        info.moduleId = id;
        info.displayName = name;
        info.description = desc;
        info.configs = std::move(configs);
        bool ok = registerModule(info);
        context.logger().info("register {} => {}", name, ok ? "ok" : "fail");
    };

    reg("killaura", "KillAura", "Automatically attacks nearby entities", {
        {"range", "Range", ConfigType::SliderFloat, "3.5", "1.0", "7.0", ""},
        {"cps", "CPS", ConfigType::SliderInt, "12", "1", "20", ""},
        {"players", "Target Players", ConfigType::Toggle, "true", "", "", ""},
    });
    reg("reach", "Reach", "Extends attack reach distance", {
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
    reg("fullbright", "Fullbright", "Maximum brightness everywhere");
    reg("scaffold", "Scaffold", "Auto-places blocks under you", {
        {"tower", "Tower Mode", ConfigType::Toggle, "true", "", "", ""},
    });
    reg("xray", "XRay", "See ores through blocks", {
        {"opacity", "Opacity", ConfigType::SliderFloat, "0.3", "0.0", "1.0", ""},
    });

    return true;
}

bool MyMod::disable(pl::mod::ModContext &context) {
    using namespace pl::modmenu;
    const char* ids[] = {"killaura","reach","fly","speed","sprint","nofall","esp","fullbright","scaffold","xray"};
    for (auto id : ids) unregisterModule(id);
    context.logger().info("Disabled");
    return true;
}

bool MyMod::unload(pl::mod::ModContext &context) {
    context.logger().info("Unloaded");
    return true;
}

} // namespace nexus
