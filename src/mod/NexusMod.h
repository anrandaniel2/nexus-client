#pragma once
#include <pl/Mod.hpp>

namespace nexus {

class NexusMod {
public:
    static NexusMod& instance();

    NexusMod();

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

private:
    ll::mod::NativeMod& mSelf;

    void registerCombatModules();
    void registerMovementModules();
    void registerRenderModules();
    void registerPlayerModules();
    void registerWorldModules();
    void registerMiscModules();
};

} // namespace nexus
