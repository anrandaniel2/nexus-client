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
        self.getLogger().error("Failed to create data directory {}: {}", self.getDataDir().string(), ec.message());
        return false;
    }
    std::filesystem::create_directories(self.getConfigDir(), ec);
    if (ec) {
        self.getLogger().error("Failed to create config directory {}: {}", self.getConfigDir().string(), ec.message());
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
    const auto &config = mConfigFile->value();

    return pl::modmenu::ModuleBuilder("nexus_client.hud", "NexusClient HUD")
        .modId(getSelf().getId())
        .description("Pure C++ lifecycle module with persistent typed config.")
        .defaultEnabled(config.enabled)
        .config("opacity", "Opacity", pl::modmenu::ConfigType::SliderInt,
                "80", "0", "100")
        .registerModule();
}

bool MyMod::disable() {
    pl::modmenu::unregisterModule("nexus_client.hud");
    return true;
}

bool MyMod::unload() {
    mConfigFile.reset();
    return true;
}

} // namespace nexus
