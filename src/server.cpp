#include <asx/log.hpp>
#include <asx/server.hpp>

#include <bln_net/utils.hpp>

#include <utility>

namespace asx {

server::server(socket& s)
  : m_socket{s}
{}

auto server::add_track(track t) -> std::string
{
    t.id = std::to_string(m_tracks.size());
    m_tracks.push_back(std::move(t));

    return m_tracks.back().id;
}

void server::recv()
{
    using namespace std;

    auto [remote, bytes] = m_socket.wait();
    auto [head, body] = unpack_msg(bln_net::to_string(bytes));

    if (head == req_tracks::name)
        recv(req_tracks{move(remote)});
}

void server::recv(req_tracks r)
{
    asx_dbg("[r|tracks]", r.client.to_string());
    send(rep_tracks{std::move(r.client), to_json(m_tracks)});
}

void server::send(rep_tracks r)
{
    asx_dbg("[s|tracks]", r.client.to_string(), to_string(r.tracks));

    m_socket.put({
        std::move(r.client),
        bln_net::to_bytes(pack_msg(rep_tracks::name, to_string(r.tracks)))
    });
}

} // namespace asx
