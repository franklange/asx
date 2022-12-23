#pragma once

#include <asx/json.hpp>
#include <asx/types.hpp>

#include <optional>
#include <string>

namespace asx {

const timeout g_timeout{50ms};

// server side
struct req_tracks
{
    remote client;
    static constexpr std::string name{"req_tracks"};
};

struct req_segment
{
    remote client;
    u16 track_id{0};
    u16 segment_id{0};

    static constexpr std::string name{"req_seg"};
};

// client side
struct rep_tracks
{
    json tracks;
    static constexpr std::string name{"rep_tracks"};
};

struct rep_segment
{
    u16 track_id{0};
    u16 segment_id{0};
    segment data;

    static constexpr std::string name{"rep_seg"};
};

// converters
auto make_req_tracks()     -> bytes;
auto make_rep_tracks(json) -> bytes;

auto make_req_segment(u16, u16) -> bytes;
auto make_rep_segment(u16, u16, const segment&) -> bytes;

auto parse_req_tracks(const json&) -> std::optional<req_tracks>;
auto parse(rep_tracks&, const json&) -> bool;

auto parse_req_segment(const json&) -> std::optional<req_segment>;
auto parse(rep_segment&, const json&) -> bool;

} // namespace asx
