#pragma once

#include <bln/types.hpp>

#include <array>
#include <vector>

namespace asx {

using namespace bln;

using Sample = float;

const u16 channels{2};
const u16 rate{48000};

const u16 bufFrames{1024};
const u16 bufSamples{bufFrames * channels};
const u16 bufBytes{bufSamples * sizeof(Sample)};

using Buffer = std::array<Sample, bufSamples>;
using Track = std::vector<Buffer>;

} // namespace asx
