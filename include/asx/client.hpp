#pragma once

#include <asx/protocol.hpp>
#include <asx/sink.hpp>
#include <asx/types.hpp>

#include <bln_net/udp_socket.hpp>
#include <bln_queue/msg_queue.hpp>

#include <optional>
#include <string>
#include <unordered_map>

namespace asx {

struct playback_info
{
    u16 size{0};
    u16 index{0};
};

class client
{
    using cmd_queue = bln_queue::msg_queue<std::string>;
    using track_id  = std::optional<u16>;
    using tracks    = std::unordered_map<u16, playback_info>;

public:
    client(socket&, remote);

    void request(std::string);

    void process_cmds();
    void fetch_tracks();
    void fetch_segments();

    auto get(rep_tracks&) -> bool;
    auto get(rep_segment&) -> bool;

    void handle(rep_tracks&);
    void handle(rep_segment&);

    void update_tracks(const json&);

private:
    auto is_current_track(const rep_segment&) -> bool;

    socket& m_socket;
    const remote m_server;

    track_id m_tid;
    cmd_queue m_cmds;

    audio_queue m_aq;
    sink m_sink{m_aq};

    tracks m_tracks;
};

} // namespace asx
