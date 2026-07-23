#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

using namespace pl::modmenu;

namespace nexus {

void NexusMod::registerRenderModules() {
    ModuleBuilder("esp", "ESP")
        .description("Shows player info through walls").modId("nexus_client")
        .config("tracers", "Tracers", ConfigType::Toggle, "false")
        .config("nametags", "Nametags", ConfigType::Toggle, "true")
        .config("health_bar", "Health Bar", ConfigType::Toggle, "true")
        .config("max_distance", "Max Distance", ConfigType::SliderFloat, "128", "16", "256")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("ESP {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("fullbright", "Fullbright")
        .description("Maximum brightness everywhere").modId("nexus_client")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Fullbright {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("nohurtcam", "NoHurtCam")
        .description("Removes camera shake when damaged").modId("nexus_client")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("NoHurtCam {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("freecam", "Freecam")
        .description("Detach camera and fly around").modId("nexus_client")
        .config("speed", "Speed", ConfigType::SliderFloat, "2.0", "0.5", "10.0")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Freecam {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("nametags", "Nametags")
        .description("Enhanced nametag rendering").modId("nexus_client")
        .config("scale", "Scale", ConfigType::SliderFloat, "1.5", "0.5", "3.0")
        .config("show_health", "Show Health", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Nametags {}", e?"ON":"OFF"); })
        .registerModule();

    ModuleBuilder("zoom", "Zoom")
        .description("Zooms in the camera view").modId("nexus_client")
        .config("fov", "FOV", ConfigType::SliderFloat, "30", "10", "90")
        .config("smooth", "Smooth", ConfigType::Toggle, "true")
        .onToggle([this](std::string_view, bool e) { getSelf().getLogger().info("Zoom {}", e?"ON":"OFF"); })
        .registerModule();
}

} // namespace nexus
