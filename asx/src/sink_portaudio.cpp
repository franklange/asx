#include <asx/sink_portaudio.hpp>
#include <bln/types.hpp>

#include <cstring>
#include <stdexcept>

namespace asx {

using namespace bln;

using Time = PaStreamCallbackTimeInfo;
using Flags = PaStreamCallbackFlags;

static void done(void* data)
{
    static_cast<PortAudioSink*>(data)->done();
}

static auto cb(const void*, void* out, const u64 frames, const Time*, const Flags, void* data) -> int
{
    return static_cast<PortAudioSink*>(data)->process(static_cast<float*>(out), frames)
        ? paContinue
        : paComplete;
}

PortAudioSink::PortAudioSink(AudioQueue& q)
    : m_queue{q}
{
    if(Pa_Initialize())
        throw std::runtime_error{"pa init"};

    if(Pa_OpenDefaultStream(&m_stream, 0, 2, paFloat32, asx::rate, asx::bufFrames, cb, this))
        throw std::runtime_error{"pa open stream"};

    if(Pa_SetStreamFinishedCallback(m_stream, asx::done))
        throw std::runtime_error{"pa set finish cb"};
}

PortAudioSink::~PortAudioSink()
{
    Pa_Terminate();
}

void PortAudioSink::start()
{
    if (Pa_StartStream(m_stream))
        throw std::runtime_error{"pa start stream"};
}

void PortAudioSink::stop()
{
    Pa_StopStream(m_stream);
}

void PortAudioSink::wait()
{
    m_done.put(m_done.wait());
}

void PortAudioSink::done()
{
    m_done.put(true);
}

auto PortAudioSink::process(float* out, const u64 frames) -> bool
{
    const auto buf = m_queue.get();

    if (!buf)
        return false;

    std::memcpy(out, buf.value().data(), asx::bufBytes);
    return true;
}

} // namespace asx
