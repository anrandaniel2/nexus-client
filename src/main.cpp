#include <jni.h>
#include <dlfcn.h>
#include <string>
#include <vector>
#include <pl/ModMenu.hpp>

namespace {
using RegisterModuleFn = bool(*)(const pl::modmenu::ModuleInfo&);
static RegisterModuleFn gRegisterModule = nullptr;

bool resolveMenuApi() {
    if (gRegisterModule) return true;
    void* sym = dlsym(RTLD_DEFAULT, "_ZN2pl7modmenu14registerModuleERKNS0_10ModuleInfoE");
    gRegisterModule = reinterpret_cast<RegisterModuleFn>(sym);
    return gRegisterModule != nullptr;
}

bool reg(const char* id, const char* name, const char* desc,
         std::vector<pl::modmenu::ConfigEntry> configs = {}) {
    if (!resolveMenuApi()) return false;
    pl::modmenu::ModuleInfo info;
    info.moduleId = id;
    info.displayName = name;
    info.description = desc;
    info.defaultEnabled = false;
    info.hideInHudEditor = false;
    info.configs = std::move(configs);
    return gRegisterModule(info);
}
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Load(JavaVM* vm, const void* modInfo) {
    (void)vm;
    (void)modInfo;
    // Register launcher-side menu metadata at load time.
    reg("killaura", "KillAura", "Automatically attacks nearby entities", {
        {"range", "Range", pl::modmenu::ConfigType::SliderFloat, "3.5", "1.0", "7.0", ""},
        {"cps", "CPS", pl::modmenu::ConfigType::SliderInt, "12", "1", "20", ""},
        {"players", "Target Players", pl::modmenu::ConfigType::Toggle, "true", "", "", ""},
    });
    reg("reach", "Reach", "Extends attack reach distance", {
        {"distance", "Distance", pl::modmenu::ConfigType::SliderFloat, "4.0", "3.0", "7.0", ""},
    });
    reg("fly", "Fly", "Allows free flight", {
        {"speed", "Speed", pl::modmenu::ConfigType::SliderFloat, "2.0", "0.5", "10.0", ""},
    });
    reg("speed", "Speed", "Increases movement speed", {
        {"multiplier", "Multiplier", pl::modmenu::ConfigType::SliderFloat, "1.5", "1.0", "5.0", ""},
    });
    reg("sprint", "Sprint", "Automatically sprints");
    reg("nofall", "NoFall", "Prevents fall damage");
    reg("esp", "ESP", "Shows player info through walls", {
        {"tracers", "Tracers", pl::modmenu::ConfigType::Toggle, "false", "", "", ""},
        {"nametags", "Nametags", pl::modmenu::ConfigType::Toggle, "true", "", "", ""},
    });
    reg("fullbright", "Fullbright", "Maximum brightness everywhere");
    reg("scaffold", "Scaffold", "Auto-places blocks under you", {
        {"tower", "Tower Mode", pl::modmenu::ConfigType::Toggle, "true", "", "", ""},
    });
    reg("xray", "XRay", "See ores through blocks", {
        {"opacity", "Opacity", pl::modmenu::ConfigType::SliderFloat, "0.3", "0.0", "1.0", ""},
    });
    return true;
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Enable() {
    return true;
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Disable() {
    return true;
}

extern "C" __attribute__((visibility("default")))
bool PLMod_Unload() {
    return true;
}
