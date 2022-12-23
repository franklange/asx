#include <asx/log.hpp>
#include <asx/server.hpp>
#include <asx/track.hpp>

#include <bln_net/udp_socket_asio.hpp>

#include <filesystem>

using dir_iter = std::filesystem::directory_iterator;

auto main() -> int
{
    bln_net::udp::socket_asio s{8000};
    asx::server srv{s};

    for (auto const& file : dir_iter{"./tracks"})
        srv.add_track(asx::from_wav(file));

    asx_log("[daemon] listening on port 8000 ...");

    while (true)
        srv.recv();

    return 0;
}
