#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

using namespace pl::modmenu;

namespace nexus {

void NexusMod::registerMovementModules() {
    ModuleBuilder("fly", "Fly")
        .description("Allows free flight").modId("nexus_client")
        .config("speed", "Speed", ConfigType::SliderFloat, "2.0", "0.5", "10.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Fly {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("speed", "Speed")
        .description("Increases movement speed").modId("nexus_client")
        .config("multiplier", "Multiplier", ConfigType::SliderFloat, "1.5", "1.0", "5.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Speed {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("sprint", "Sprint")
        .description("Automatically sprints").modId("nexus_client")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Sprint {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("nofall", "NoFall")
        .description("Prevents fall damage").modId("nexus_client")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("NoFall {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("jesus", "Jesus")
        .description("Walk on water and lava").modId("nexus_client")
        .config("lava", "Include Lava", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Jesus {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("step", "Step")
        .description("Step up blocks instantly").modId("nexus_client")
        .config("height", "Height", ConfigType::SliderFloat, "1.0", "0.5", "4.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Step {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("longjump", "LongJump")
        .description("Jump further than normal").modId("nexus_client")
        .config("boost", "Boost", ConfigType::SliderFloat, "1.5", "0.5", "5.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("LongJump {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("spider", "Spider")
        .description("Climb walls like a spider").modId("nexus_client")
        .config("speed", "Climb Speed", ConfigType::SliderFloat, "0.2", "0.05", "1.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Spider {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("glide", "Glide")
        .description("Slowly glide downwards").modId("nexus_client")
        .config("speed", "Glide Speed", ConfigType::SliderFloat, "0.1", "0.01", "0.5")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Glide {}", e?"ON":"OFF"); })
        .registerModule();
}

} // namespace nexus
