#include <asx/client.hpp>

#include <bln_net/udp_socket_asio.hpp>

#include <iostream>
#include <string>
#include <thread>

using socket_t = bln_net::udp::socket_asio;

void cmdline(asx::client& c)
{
    std::string input;

    while (std::getline(std::cin, input))
        c.request(input);
}

auto main() -> int
{
    socket_t s{9000};
    asx::client c{s, {"127.0.0.1", 8000}};

    std::thread ui_runner{cmdline, std::ref(c)};

    while (true)
        c.process_cmds();

    ui_runner.join();

    return 0;
}
