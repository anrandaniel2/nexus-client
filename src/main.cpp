#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include <pl/ModMenu.hpp>

#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "NexusClient", __VA_ARGS__)

namespace {
void reg(const char* id, const char* name, const char* desc,
         std::vector<pl::modmenu::ConfigEntry> configs = {}) {
    pl::modmenu::ModuleInfo info;
    info.moduleId = id;
    info.displayName = name;
    info.description = desc;
    info.defaultEnabled = false;
    info.configs = std::move(configs);
    bool ok = pl::modmenu::registerModule(info);
    LOG("register %s => %s", name, ok ? "ok" : "fail");
}
}

// Use PLMod_Load lifecycle — NOT PL_REGISTER_MOD
// This avoids NativeMod::current() and uses the launcher's lifecycle path
extern "C" __attribute__((visibility("default")))
bool PLMod_Load(JavaVM* vm, const void* modInfo) {
    (void)vm;
    (void)modInfo;
    LOG("NexusClient PLMod_Load called");
    return true;
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Enable() {
    LOG("NexusClient PLMod_Enable called");

    using pl::modmenu::ConfigType;

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

    LOG("NexusClient: all modules registered");
    return true;
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Disable() {
    LOG("NexusClient PLMod_Disable called");
    return true;
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Unload() {
    LOG("NexusClient PLMod_Unload called");
    return true;
}
