#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

using namespace pl::modmenu;

namespace nexus {

void NexusMod::registerMiscModules() {
    ModuleBuilder("disabler", "Disabler")
        .description("Attempts to disable server anti-cheat").modId("nexus_client")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Disabler {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("spammer", "Spammer")
        .description("Automatically sends chat messages").modId("nexus_client")
        .config("delay", "Delay (ms)", ConfigType::SliderInt, "3000", "500", "10000")
        .config("randomize", "Randomize", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Spammer {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("selfdestruct", "SelfDestruct")
        .description("Unloads client and removes traces").modId("nexus_client")
        .onToggle([this](std::string_view, bool enabled) {
            if (enabled) {
                getSelf().getLogger().info("SelfDestruct triggered!");
                pl::modmenu::setModuleEnabled("selfdestruct", false);
            }
        })
        .registerModule();
}

} // namespace nexus
