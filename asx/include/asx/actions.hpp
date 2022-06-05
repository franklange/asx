#pragma once

#include <asx/audio.hpp>
#include <variant>

namespace asx {

struct Stop{};
struct Toggle{};
struct Quit{};

struct SetTrack
{
    Track track;
};

using Action = std::variant<
    std::monostate,
    Stop,
    Toggle,
    Quit,
    SetTrack
>;

} // namespace asx
