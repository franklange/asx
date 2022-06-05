#include <asx/match.hpp>
#include <asx/playback_worker.hpp>

#include <iostream>

namespace asx {

PlaybackWorker::PlaybackWorker(ActionQueue& q, AudioQueue& a)
    : m_queue{q}
    , m_pb{a}
    , m_thread{&PlaybackWorker::run, this}
{}

PlaybackWorker::~PlaybackWorker()
{
    if (m_thread.joinable())
        m_thread.join();
}

void PlaybackWorker::run()
{
    while (true)
    {
        const auto action = m_queue.wait(20ms);

        if (!action)
        {
            if (m_pb.is_playing())
                m_pb.push();

            continue;
        }

        if (std::holds_alternative<Quit>(action.value()))
            break;

        std::visit(match{
            [&](const Stop& a)   { stop(a); },
            [&](const Toggle& a) { toggle(a); },
            [&](SetTrack a)    { setTrack(std::move(a)); },
            [](auto...) {}
        }, action.value());
    }
}

void PlaybackWorker::setTrack(Action&& a)
{
    m_pb.process(std::move(a));
}

void PlaybackWorker::stop(const Action& a)
{
    if (m_pb.is_stopped())
        return;

    m_pb.process(a);
}

void PlaybackWorker::toggle(const Action& a)
{
    if (!m_pb.has_track())
        return;

    m_pb.process(a);
}

} // namespace asx
