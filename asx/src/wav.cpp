#include <asx/wav.hpp>
#include <bln/types.hpp>

#include <sndfile.h>
#include <stdexcept>

#include <iostream>

namespace asx {

auto load_wav(const Path& p) -> Track
{
    SF_INFO info;

    auto* f = sf_open(p.c_str(), SFM_READ, &info);

    if (!f)
        throw std::runtime_error{"couldn't open file"};


    Track track(info.frames / asx::bufFrames);

    for (auto& buf : track)
        sf_readf_float(f, buf.data(), asx::bufFrames);

    sf_close(f);

    return track;
}

} // namespace asx
