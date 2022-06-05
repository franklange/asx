#pragma once

#include <asx/audio.hpp>

#include <filesystem>
#include <vector>

namespace asx {

using Path = std::filesystem::path;

auto load_wav(const Path&) -> Track;

} // namespace asx
