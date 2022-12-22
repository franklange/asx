#pragma once

#include <asx/message.hpp>
#include <asx/track.hpp>
#include <asx/types.hpp>

#include <string>

namespace asx {

class server
{

public:
    server(socket&);

    auto add_track(track) -> std::string;

    void recv();
    void recv(req_tracks);
    void recv(req_segment);

    void send(rep_tracks);
    void send(rep_segment);

private:
    socket& m_socket;
    tracks m_tracks;
};

} // namespace asx
