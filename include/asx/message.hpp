#pragma once

#include <asx/json.hpp>
#include <asx/types.hpp>
#include <asx/track.hpp>

#include <string>

namespace asx {

struct message
{
    std::string head;
    std::string body;
};

auto pack_msg(std::string, std::string) -> std::string;
auto unpack_msg(const std::string&) -> message;

struct req_tracks
{
    static constexpr std::string name{"req_tracks"};

    remote client;
};

struct rep_tracks
{
    static constexpr std::string name{"rep_tracks"};

    remote client;
    json tracks;
};

struct req_segment
{
    static constexpr std::string name{"req_seg"};

    std::string track_id;
    std::string segment_id;
};

struct rep_segment
{
    static constexpr std::string name{"rep_seg"};

    remote client;
    json segment;
};

} // namespace asx
