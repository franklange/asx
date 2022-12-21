#pragma once

#include <asx/types.hpp>

#include <filesystem>

namespace asx {

using path = std::filesystem::path;

auto from_wav(const path&) -> track;

} // namespace asx
