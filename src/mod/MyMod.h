#pragma once

#include <pl/Mod.hpp>
#include <pl/Config.hpp>
#include "mod/Config.h"
#include <optional>

namespace nexus {

class MyMod {
public:
    static MyMod &instance();

    MyMod();

    [[nodiscard]] ll::mod::NativeMod &getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod &mSelf;
    ModConfig mConfig;
    std::optional<pl::config::ConfigFile<ModConfig>> mConfigFile;
};

} // namespace nexus
