#include <asx/sink_portaudio.hpp>
#include <bln/types.hpp>

#include <algorithm>
#include <cstring>
#include <stdexcept>

#include <stdio.h>

namespace asx {

using namespace bln;

using Time = PaStreamCallbackTimeInfo;
using Flags = PaStreamCallbackFlags;

static void done_cb(void* data)
{
    static_cast<PortAudioSink*>(data)->done();
}

static auto process_cb(const void*, void* out, const unsigned long frames, const Time*, const Flags, void* data) -> int
{
    return static_cast<PortAudioSink*>(data)->process(static_cast<float*>(out), frames)
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

PortAudioSink::PortAudioSink(AudioQueue& q)
    : m_queue{q}
{
    if(pa_quiet_init())
        throw std::runtime_error{"pa init"};

    if(Pa_OpenDefaultStream(&m_stream, 0, 2, paFloat32, asx::rate, asx::bufFrames, process_cb, this))
        throw std::runtime_error{"pa open stream"};

    if(Pa_SetStreamFinishedCallback(m_stream, done_cb))
        throw std::runtime_error{"pa set finish cb"};
}

PortAudioSink::~PortAudioSink()
{
    Pa_Terminate();
}

void PortAudioSink::start()
{
    m_active = true;
    Pa_StartStream(m_stream);
}

void PortAudioSink::pause()
{
    m_active = false;
}

void PortAudioSink::stop()
{
    Pa_StopStream(m_stream);
    m_active = false;
}

void PortAudioSink::finish()
{
    m_done.put(m_done.wait());
}

auto PortAudioSink::toggle() -> bool
{
    if (active())
        pause();
    else
        start();

    return active();
}

auto PortAudioSink::active() const -> bool
{
    return m_active;
}

void PortAudioSink::done()
{
    m_done.put(true);
}

auto PortAudioSink::process(float* out, const u64 frames) -> bool
{
    if (!m_active)
    {
        std::fill_n(out, asx::bufSamples, 0);
        return true;
    }

    const auto buf = m_queue.get();

    if (!buf)
        return false;

    std::memcpy(out, buf.value().data(), asx::bufBytes);
    return true;
}

} // namespace asx
