#pragma once

#include <asx/sink.hpp>
#include <asx/queue.hpp>

#include <bln/types.hpp>

#include <portaudio.h>

namespace asx {

class PortAudioSink : public Sink
{
public:
    PortAudioSink(AudioQueue&);
    ~PortAudioSink();

    void start() override;
    void stop() override;

    void wait();

    void done();
    auto process(float*, u64) -> bool;

private:
    AudioQueue& m_queue;
    PaStream* m_stream;
    Semaphore m_done{2};
};

} // namespace asx
