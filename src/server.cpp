#include <asx/log.hpp>
#include <asx/server.hpp>

#include <bln_net/utils.hpp>

#include <cassert>
#include <utility>

namespace asx {

server::server(socket& s)
  : m_socket{s}
{}

auto server::add_track(track t) -> u16
{
    const auto id = m_tracks.size();

    t.id = id;
    m_tracks.push_back(std::move(t));

    auto& nt = m_tracks.back();
    asx_log("[add_track]", nt.name, nt.data.size());

    return id;
}

void server::recv()
{
    using namespace std;

    auto [remote, bytes] = m_socket.wait();
    const json req = from_bytes(bytes);

    const auto& head = req["head"];
    const auto& body = req["body"];

    if (head == req_tracks::name)
    {
        auto req = parse_req_tracks(body);
        if (!req)
        {
            asx_log("[recv] [req_tracks] [err] invalid request");
            return;
        }

        req.value().client = std::move(remote);
        process(std::move(req.value()));

        return;
    }

    if (head == req_segment::name)
    {
        auto req = parse_req_segment(body);
        if (!req)
        {
            asx_log("[recv] [req_segment] [err] invalid request");
            return;
        }

        req.value().client = std::move(remote);
        process(std::move(req.value()));

        return;
    }

    asx_log("[recv] [err]", "unknown message");
}

void server::process(req_tracks r)
{
    m_socket.put({
        std::move(r.client),
        make_rep_tracks(to_json(m_tracks))
    });
}

void server::process(req_segment r)
{
    m_socket.put({
        std::move(r.client),
        make_rep_segment(
            r.track_id,
            r.segment_id,
            m_tracks.at(r.track_id).data.at(r.segment_id))
    });
}

} // namespace asx
