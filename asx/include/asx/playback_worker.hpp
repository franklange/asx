#pragma once

#include <asx/queue.hpp>
#include <asx/playback.hpp>

#include <thread>

namespace asx {

class PlaybackWorker
{
    using Flag = std::atomic_bool;

public:
    PlaybackWorker(ActionQueue&, AudioQueue&);
    ~PlaybackWorker();

private:
    void run();

    void setTrack(Action&&);
    void stop(const Action&);
    void toggle(const Action&);

    ActionQueue& m_queue;
    Playback m_pb;
    std::thread m_thread;
};

} // namespace asx
