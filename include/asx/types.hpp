#pragma once

#include <bln_queue/data_queue.hpp>

#include <array>
#include <cstdint>
#include <vector>

namespace asx {

using u16 = std::uint16_t;
using u64 = std::uint64_t;

using sample = float;

const u16 channels{2};
const u16 rate{48000};

const u16 seg_frames{256};
const u16 seg_samples{seg_frames * channels};
const u16 seg_bytes{seg_samples * sizeof(sample)};

using segment = std::array<sample, seg_samples>;
using track = std::vector<segment>;

using audio_queue = bln_queue::data_queue<segment, 128>;

} // namespace asx
