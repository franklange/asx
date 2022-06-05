#pragma once

#include <asx/queue.hpp>

#include <bln/types.hpp>
#include <portaudio.h>

namespace asx {

class PortAudioSink
{
public:
    PortAudioSink(AudioQueue&);
    ~PortAudioSink();

    void start();
    void stop();

    auto process(float*, u64) -> bool;

private:
    AudioQueue& m_queue;
    PaStream* m_stream;
};

} // namespace asx
