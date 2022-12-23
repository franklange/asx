#include <asx/track.hpp>
#include <asx/log.hpp>

#include <sndfile.h>

#include <iterator>
#include <stdexcept>

namespace asx {

auto from_wav(const path& p) -> track
{
    SF_INFO info;

    auto* f = sf_open(p.c_str(), SFM_READ, &info);

    if (!f)
        throw std::runtime_error{"couldn't open file"};

    track res;
    res.name = p.stem();
    res.data.resize(info.frames / asx::seg_frames);

    for (auto& seg : res.data)
        sf_readf_float(f, seg.data(), asx::seg_frames);

    sf_close(f);

    return res;
}

auto track::to_json() const -> json
{
    return {
        {"id", id},
        {"name", name},
        {"size", data.size()},
    };
}

auto to_json(const tracks& ts) -> json
{
    json res;

    std::transform(ts.begin(), ts.end(), std::back_inserter(res), [](const auto& t){
        return t.to_json();
    });

    return res;
}

auto to_json(const segment& s) -> json
{
    return {};
}

} // namespace asx
