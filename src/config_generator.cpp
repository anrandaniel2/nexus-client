#include "mod/Config.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace {

bool writeJson(const std::filesystem::path &path, const nlohmann::json &value) {
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
        std::cerr << "mkdir " << path.parent_path() << ": " << ec.message() << "\n";
        return false;
    }
    std::ofstream ofs(path);
    if (!ofs) {
        std::cerr << "open " << path << " failed\n";
        return false;
    }
    ofs << value.dump(2) << "\n";
    return true;
}

} // namespace

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <output-dir>\n";
        return 2;
    }

    const std::filesystem::path outputDir = argv[1];

    if (!writeJson(outputDir / "config.json", nexus::makeDefaultConfigJson()))
        return 1;
    if (!writeJson(outputDir / "config.schema.json", nexus::makeConfigSchemaJson()))
        return 1;

    return 0;
}
