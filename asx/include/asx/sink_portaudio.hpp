#pragma once

#include <asx/queue.hpp>

#include <bln/types.hpp>
#include <portaudio.h>

#include <atomic>

namespace asx {

class PortAudioSink
{
    using Flag = std::atomic_bool;

public:
    PortAudioSink(AudioQueue&);
    ~PortAudioSink();

    void start();
    void pause();
    void stop();
    void finish();

    auto toggle() -> bool;
    auto active() const -> bool;

    void done();
    auto process(float*, u64) -> bool;

private:
    AudioQueue& m_queue;
    PaStream* m_stream;
    Semaphore m_done{2};
    Flag m_active{false};
};

} // namespace asx
