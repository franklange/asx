#pragma once

#include <asx/json.hpp>
#include <asx/types.hpp>

#include <string>
#include <vector>

namespace asx {

struct track
{
    std::string id;
    std::string name;
    segments    data;

    auto to_json() const -> json;
};

using tracks = std::vector<track>;

auto from_wav(const path&) -> track;
auto to_json(const tracks&) -> json;

} // namespace asx
