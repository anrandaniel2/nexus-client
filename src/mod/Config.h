#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pl/Config.hpp>

namespace nexus {

struct ModConfig {
    int version = 1;
    bool enabled = true;
    std::string message = "Hello from NexusClient";
};

nlohmann::json makeDefaultConfigJson();
nlohmann::json makeConfigSchemaJson();

} // namespace nexus

template <>
struct pl::config::Schema<nexus::ModConfig> {
    static constexpr std::string_view title       = "NexusClient Config";
    static constexpr std::string_view description = {};

    static constexpr FieldSchema field(std::string_view name) {
        if (name == "version")
            return {.title = "Version", .readOnly = true};
        if (name == "enabled")
            return {.title = "Enabled", .description = "Turns NexusClient on or off."};
        if (name == "message")
            return {.title = "Message", .description = "Message written when the mod is enabled."};
        return {};
    }
};
