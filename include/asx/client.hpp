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
    using track_id  = std::optional<u16>;
    using tracks    = std::unordered_map<u16, playback_info>;

public:
    client(remote, socket&, cmd_queue&, audio_queue&);

    void process_cmd();
    void fetch_tracks();
    void fetch_segments();

    auto get(rep_tracks&) -> bool;
    auto get(rep_segment&) -> bool;

    void handle(rep_tracks&);
    void handle(rep_segment&);

    void update_tracks(const json&);
    auto is_current_track(const rep_segment&) -> bool;

private:
    const remote m_server;

    socket& m_socket;
    cmd_queue& m_cmds;
    audio_queue& m_audio;

    track_id m_tid;
    tracks m_tracks;
};

} // namespace asx
