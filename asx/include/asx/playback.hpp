#pragma once

#include <asx/audio.hpp>
#include <asx/queue.hpp>

#include <bln/types.hpp>

namespace asx {

class Playback
{
public:
    Playback(AudioQueue&);

    void push();
    void process(Action);

    auto is_playing() const -> bool;
    auto is_paused()  const -> bool;
    auto is_stopped() const -> bool;
    auto has_track() const -> bool;

private:
    void play();
    void pause();
    void stop();
    void toggle();
    void setTrack(Track);

    AudioQueue& m_queue;

    bool m_active{false};

    Track m_track;
    bln::u16 m_index{0};
};

} // namespace asx
