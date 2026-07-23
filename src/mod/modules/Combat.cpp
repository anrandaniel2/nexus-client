#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

using namespace pl::modmenu;

namespace nexus {

static bool s_killauraEnabled = false;
static float s_killauraRange = 3.5f;
static int s_killauraCPS = 12;
static float s_reachDistance = 4.0f;

void NexusMod::registerCombatModules() {
    std::string mid(getSelf().getId());

    ModuleBuilder("killaura", "KillAura")
        .description("Automatically attacks nearby entities")
        .modId(mid)
        .config("range", "Range", ConfigType::SliderFloat, "3.5", "1.0", "7.0")
        .config("cps", "CPS", ConfigType::SliderInt, "12", "1", "20")
        .config("players", "Target Players", ConfigType::Toggle, "true")
        .config("mobs", "Target Mobs", ConfigType::Toggle, "false")
        .config("rotation", "Rotate to Target", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool enabled) {
            s_killauraEnabled = enabled;
            getSelf().getLogger().info("KillAura {}", enabled ? "ON" : "OFF");
        })
        .onConfigChanged([this](std::string_view, std::string_view key, std::string_view value) {
            if (key == "range") s_killauraRange = std::stof(std::string(value));
            if (key == "cps") s_killauraCPS = std::stoi(std::string(value));
        })
        .registerModule();

    ModuleBuilder("reach", "Reach")
        .description("Extends attack reach distance")
        .modId("nexus_client")
        .config("distance", "Distance", ConfigType::SliderFloat, "4.0", "3.0", "7.0")
        .onToggle([this](std::string_view, bool enabled) {
            getSelf().getLogger().info("Reach {}", enabled ? "ON" : "OFF");
        })
        .onConfigChanged([](std::string_view, std::string_view key, std::string_view value) {
            if (key == "distance") s_reachDistance = std::stof(std::string(value));
        })
        .registerModule();

    ModuleBuilder("criticals", "Criticals")
        .description("Forces critical hits on every attack")
        .modId("nexus_client")
        .onToggle([this](std::string_view, bool e) {
            getSelf().getLogger().info("Criticals {}", e ? "ON" : "OFF");
        })
        .registerModule();

    ModuleBuilder("autoclicker", "AutoClicker")
        .description("Automatically clicks at set intervals")
        .modId("nexus_client")
        .config("min_cps", "Min CPS", ConfigType::SliderInt, "8", "1", "20")
        .config("max_cps", "Max CPS", ConfigType::SliderInt, "14", "1", "20")
        .config("randomize", "Randomize", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) {
            getSelf().getLogger().info("AutoClicker {}", e ? "ON" : "OFF");
        })
        .registerModule();

    ModuleBuilder("antiknockback", "AntiKnockback")
        .description("Reduces or eliminates knockback")
        .modId("nexus_client")
        .config("horizontal", "Horizontal %", ConfigType::SliderInt, "0", "0", "100")
        .config("vertical", "Vertical %", ConfigType::SliderInt, "0", "0", "100")
        .onToggle([this](std::string_view, bool e) {
            getSelf().getLogger().info("AntiKnockback {}", e ? "ON" : "OFF");
        })
        .registerModule();

    ModuleBuilder("velocity", "Velocity")
        .description("Controls knockback velocity")
        .modId("nexus_client")
        .config("horizontal", "Horizontal", ConfigType::SliderFloat, "0.0", "-1.0", "1.0")
        .config("vertical", "Vertical", ConfigType::SliderFloat, "0.0", "-1.0", "1.0")
        .onToggle([this](std::string_view, bool e) {
            getSelf().getLogger().info("Velocity {}", e ? "ON" : "OFF");
        })
        .registerModule();
}

} // namespace nexus
