#include <asx/queue.hpp>
#include <asx/sink_portaudio.hpp>
#include <asx/wav.hpp>

#include <chrono>
#include <thread>

using namespace asx;
using namespace std::chrono_literals;

auto main() -> int
{
    AudioQueue q;
    PortAudioSink sink{q};

    auto track = load_wav("data/piano_f32.wav");

    for (auto& buf : track)
        q.put(std::move(buf));

    sink.start();
    sink.wait();

    return 0;
}
