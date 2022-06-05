#include "cmdline.hpp"

#include <asx/actions.hpp>
#include <asx/playback_worker.hpp>
#include <asx/queue.hpp>
#include <asx/sink_portaudio.hpp>
#include <asx/wav.hpp>

using namespace asx;
using namespace app;

auto main() -> int
{
    AudioQueue audio;

    PortAudioSink sink{audio};
    sink.start();

    ActionQueue actions;
    actions.put(SetTrack{load_wav("data/piano_f32.wav")});

    Cmdline cmdline{actions};
    PlaybackWorker playback{actions, audio};

    return 0;
}
