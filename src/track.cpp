#include <asx/track.hpp>

#include <sndfile.h>
#include <stdexcept>

namespace asx {

auto from_wav(const path& p) -> track
{
    SF_INFO info;

    auto* f = sf_open(p.c_str(), SFM_READ, &info);

    if (!f)
        throw std::runtime_error{"couldn't open file"};


    track res(info.frames / asx::seg_frames);

    for (auto& seg : res)
        sf_readf_float(f, seg.data(), asx::seg_frames);

    sf_close(f);

    return res;
}

} // namespace asx
