#pragma once

#include <asx/types.hpp>
#include <portaudio.h>

namespace asx {

class sink
{
public:
    sink(audio_queue&);
    ~sink();

    auto process(float*, u64) -> bool;

private:
    audio_queue& m_queue;
    PaStream* m_stream;
};

} // namespace asx
