#include <asx/actions.hpp>
#include <asx/match.hpp>
#include <asx/playback.hpp>

#include <cassert>
#include <type_traits>
#include <variant>

namespace asx {

Playback::Playback(AudioQueue& a)
    : m_queue{a}
{}

void Playback::push()
{
    assert(is_playing());
    assert(m_index < m_track.size());

    m_queue.put(m_track.at(m_index++));

    if (m_index >= m_track.size())
        stop();
}

void Playback::process(Action action)
{
    std::visit(match{
        [&](const Stop&)  { stop();  },
        [&](const Toggle&){ toggle(); },
        [&](SetTrack a) { setTrack(std::move(a.track)); },
        [](auto...) {}
    }, action);
}

auto Playback::is_playing() const -> bool
{
    return m_active;
}

auto Playback::is_paused() const -> bool
{
    return (!m_active && m_index);
}

auto Playback::is_stopped() const -> bool
{
    return (!m_active && !m_index);
}

auto Playback::has_track() const -> bool
{
    return !m_track.empty();
}

void Playback::play()
{
    assert(!is_playing());
    assert(!m_track.empty());

    m_active = true;
    push();
}

void Playback::pause()
{
    assert(!is_paused());
    m_active = false;
}

void Playback::stop()
{
    assert(!is_stopped());

    m_index = 0;
    m_active = false;
}

void Playback::toggle()
{
    if (is_paused() || is_stopped())
        play();
    else
        pause();
}

void Playback::setTrack(Track t)
{
    stop();
    m_track = std::move(t);
}

} // namespace asx
