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

    // Test: one module, no lambdas
    pl::modmenu::ModuleInfo info;
    info.moduleId = "test_module";
    info.displayName = "Test Module";
    info.description = "Testing if registerModule works";
    info.modId = std::string(self.getId());
    info.defaultEnabled = false;
    bool ok = pl::modmenu::registerModule(info);
    self.getLogger().info("registerModule returned: {}", ok);

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
