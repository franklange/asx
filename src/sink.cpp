#include <asx/sink.hpp>
#include <asx/types.hpp>

#include <algorithm>
#include <cstring>
#include <stdexcept>

#include <stdio.h>

namespace asx {

using Time  = PaStreamCallbackTimeInfo;
using Flags = PaStreamCallbackFlags;

static auto process_cb(const void*, void* out, const u64 frames, const Time*, const Flags, void* data) -> int
{
    return static_cast<sink*>(data)->process(static_cast<sample*>(out), frames)
        ? paContinue
        : paComplete;
}

static auto pa_quiet_init() -> PaError
{
    freopen("/dev/null","w", stderr);
    const auto ret = Pa_Initialize();
    freopen("/dev/tty","w", stderr);

    return ret;
}

sink::sink(audio_queue& q)
    : m_queue{q}
{
    if(pa_quiet_init())
        throw std::runtime_error{"pa init"};

    if(Pa_OpenDefaultStream(&m_stream, 0, 2, paInt16, asx::rate, asx::seg_frames, process_cb, this))
        throw std::runtime_error{"pa open stream"};

    Pa_StartStream(m_stream);
}

sink::~sink()
{
    Pa_StopStream(m_stream);
    Pa_Terminate();
}

auto sink::process(sample* out, const u64) -> bool
{
    const auto buf = m_queue.get();

    if (!buf)
        std::fill_n(out, asx::seg_samples, 0);
    else
        std::memcpy(out, buf.value().data(), asx::seg_bytes);

    return true;
}

} // namespace asx
