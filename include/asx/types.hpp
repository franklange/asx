#pragma once

#include <bln_queue/data_queue.hpp>
#include <bln_net/udp_socket.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <vector>

namespace asx {

using namespace std::chrono_literals;

using u16 = std::uint16_t;
using u64 = std::uint64_t;

// audio
using sample = float;

const u16 channels{2};
const u16 rate{48000};

const u16 seg_frames{64};
const u16 seg_samples{seg_frames * channels};
const u16 seg_bytes{seg_samples * sizeof(sample)};

using segment  = std::array<sample, seg_samples>;
using segments = std::vector<segment>;

using audio_queue = bln_queue::data_queue<segment, 1024>;

// fs
using path = std::filesystem::path;

// network
using bytes   = bln_net::bytes;
using socket  = bln_net::udp::socket;
using packet  = bln_net::udp::packet;
using remote  = bln_net::udp::endpoint;
using timeout = std::chrono::milliseconds;

} // namespace asx
