#include <asx/client.hpp>
#include <asx/log.hpp>

#include <bln_net/utils.hpp>

#include <cctype>
#include <string_view>

namespace asx {

namespace {

auto is_tid(const std::string_view s) -> bool
{
    return (s.size() == 1) && std::isdigit(static_cast<unsigned char>(s.at(0)));
}

auto is_stop(const std::string_view s) -> bool
{
    return ((s == "pause") || (s == "stop"));
}

} // namespace anonym

client::client(remote r, socket& s, cmd_queue& c, audio_queue& a)
    : m_server{r}
    , m_socket{s}
    , m_cmds{c}
    , m_audio{a}
{
    fetch_tracks();
}

void client::process_cmd()
{
    const auto cmd = m_cmds.wait();

    if (cmd == "tracks")
    {
        fetch_tracks();
    }
    else if (is_tid(cmd))
    {
        m_tid = std::stoi(cmd);
        fetch_segments();
    }
}

void client::fetch_tracks()
{
    rep_tracks r;
    if (!get(r))
    {
        asx_log("[fetch_tracks] [err] fetch error");
        return;
    }

    handle(r);
}

void client::fetch_segments()
{
    rep_segment r;

    while (true)
    {
        if (!get(r))
        {
            asx_log("[fetch_segments] [err] fetch error");
            return;
        }

        if (!is_current_track(r))
        {
            asx_log("[fetch_segments] [err] segment not for current track");
            return;
        }

        handle(r);

        auto& pb_info = m_tracks.at(m_tid.value());


        if (pb_info.index >= pb_info.size)
        {
            asx_log("[fetch_segments] done");
            m_tracks.at(m_tid.value()).index = 0;
            m_tid.reset();
            return;
        }

        const auto cmd = m_cmds.wait(500us);

        if (cmd && is_stop(cmd.value()))
            return;
    }
}

auto client::get(rep_tracks& r) -> bool
{
    asx_dbg("[get] [tracks]");
    m_socket.put({m_server, make_req_tracks()});

    auto p = m_socket.wait(g_timeout);

    if (!p)
    {
        asx_log("[get] [tracks] [err] timeout");
        return false;
    }

    const json rep = from_bytes(p.value().data);

    if (rep["head"] != rep_tracks::name)
    {
        asx_log("[get] [tracks] [err] reply unknown");
        return false;
    }

    if (!parse(r, rep["body"]))
        asx_log("[get] [tracks] [err] reply invalid");

    return true;
}

auto client::get(rep_segment& r) -> bool
{
    const auto& pb_info = m_tracks[m_tid.value()];
    m_socket.put({m_server, make_req_segment(m_tid.value(), pb_info.index)});

    auto p = m_socket.wait(g_timeout);

    if (!p)
    {
        asx_log("[get] [segment] [err] timeout");
        return false;
    }

    const json rep = from_bytes(p.value().data);

    if (rep["head"] != rep_segment::name)
    {
        asx_log("[get] [segment] [err] reply unknown");
        return false;
    }

    if (!parse(r, rep["body"]))
        asx_log("[get] [segment]] [err] reply invalid");

    return true;
}

void client::handle(rep_tracks& r)
{
    asx_log("[handle] [tracks]", to_string(r.tracks, 2));
    update_tracks(r.tracks);
}

void client::handle(rep_segment& r)
{
    m_audio.put(std::move(r.data));

    auto& pb_info = m_tracks.at(m_tid.value());
    pb_info.index++;
}

void client::update_tracks(const json& tracks)
{
    for (const auto& t : tracks)
        m_tracks[t["id"]].size = t["size"];
}

auto client::is_current_track(const rep_segment& r) -> bool
{
    const auto& pb_info = m_tracks.at(m_tid.value());
    return (r.track_id == m_tid.value()) && (r.segment_id == pb_info.index);
}

} // namespace asx
