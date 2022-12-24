#include <asx/client.hpp>
#include <asx/types.hpp>
#include <asx/sink.hpp>

#include <bln_net/udp_socket_asio.hpp>

#include <iostream>
#include <string>
#include <thread>

void ui(asx::cmd_queue& q)
{
    std::string cmd;

    while (std::getline(std::cin, cmd))
        q.put(cmd);
}

auto main() -> int
{
    bln_net::udp::socket_asio socket{9000};

    asx::cmd_queue cmd_q;
    asx::audio_queue audio_q;

    asx::sink sink{audio_q};

    std::thread ui_runner{ui, std::ref(cmd_q)};

    asx::client c{{"192.168.1.10", 8000}, socket, cmd_q, audio_q};

    while (true)
        c.process_cmd();

    ui_runner.join();

    return 0;
}
