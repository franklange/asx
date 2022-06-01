#include <asx/queue.hpp>
#include <asx/sink_portaudio.hpp>
#include <asx/wav.hpp>

#include <chrono>
#include <string>
#include <thread>
#include <utility>

using namespace asx;
using namespace std::chrono_literals;

void cmdline(PortAudioSink& s)
{
    std::string input;

    while (std::getline(std::cin, input))
    {
        if (input == "q")
            break;

        if (input == "")
            s.toggle();
    }

    s.stop();
}

auto main() -> int
{
    AudioQueue q;
    PortAudioSink sink{q};

    auto track = load_wav("data/piano_f32.wav");

    for (auto& buf : track)
        q.put(std::move(buf));

    std::thread cliRunner{cmdline, std::ref(sink)};

    sink.finish();

    cliRunner.join();

    return 0;
}
