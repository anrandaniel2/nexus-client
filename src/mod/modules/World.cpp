#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

using namespace pl::modmenu;

namespace nexus {

void NexusMod::registerWorldModules() {
    ModuleBuilder("xray", "XRay")
        .description("See ores through blocks").modId("nexus_client")
        .config("opacity", "Block Opacity", ConfigType::SliderFloat, "0.3", "0.0", "1.0")
        .config("diamond", "Diamond", ConfigType::Toggle, "true")
        .config("gold", "Gold", ConfigType::Toggle, "true")
        .config("iron", "Iron", ConfigType::Toggle, "true")
        .config("ancient_debris", "Ancient Debris", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("XRay {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("timer", "Timer")
        .description("Changes game tick speed").modId("nexus_client")
        .config("speed", "Speed", ConfigType::SliderFloat, "2.0", "0.1", "10.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Timer {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("weather", "Weather")
        .description("Control weather rendering").modId("nexus_client")
        .config("mode", "Mode", ConfigType::Radio, "clear")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Weather {}", e?"ON":"OFF"); })
        .registerModule();
}

} // namespace nexus
