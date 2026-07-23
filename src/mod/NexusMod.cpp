#include "mod/NexusMod.h"
#include <pl/ModMenu.hpp>

namespace nexus {

NexusMod& NexusMod::instance() {
    static NexusMod inst;
    return inst;
}

NexusMod::NexusMod() : mSelf(*ll::mod::NativeMod::current()) {}

bool NexusMod::load() {
    return true;
}

bool NexusMod::enable() {
    // Bare minimum — no fmt, no logging, just register
    pl::modmenu::ModuleInfo info;
    info.moduleId = "nexus_test";
    info.displayName = "NexusClient Test";
    info.description = "Test module";
    info.modId = std::string(getSelf().getId());
    info.defaultEnabled = false;
    pl::modmenu::registerModule(info);
    return true;
}

bool NexusMod::disable() {
    pl::modmenu::unregisterModule("nexus_test");
    return true;
}

bool NexusMod::unload() {
    return true;
}

} // namespace nexus
