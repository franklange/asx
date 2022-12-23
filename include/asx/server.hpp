#pragma once

#include <asx/protocol.hpp>
#include <asx/track.hpp>
#include <asx/types.hpp>

#include <optional>
#include <string>

namespace asx {

class server
{

public:
    server(socket&);

    void recv();

    void process(req_tracks);
    void process(req_segment);

    auto add_track(track) -> u16;

private:
    socket& m_socket;
    tracks m_tracks;
};

} // namespace asx
