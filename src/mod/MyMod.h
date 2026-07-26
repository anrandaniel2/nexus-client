#pragma once

#include <pl/Mod.hpp>

namespace nexus {

class MyMod {
public:
    static MyMod &instance();

    bool load(pl::mod::ModContext &context);
    bool enable(pl::mod::ModContext &context);
    bool disable(pl::mod::ModContext &context);
    bool unload(pl::mod::ModContext &context);
};

} // namespace nexus
