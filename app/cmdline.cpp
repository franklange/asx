#include "cmdline.hpp"

#include <asx/actions.hpp>

#include <string>
#include <iostream>

namespace app {

Cmdline::Cmdline(ActionQueue& q)
    : m_queue{q}
    , m_thread{&Cmdline::run, this}
{}

Cmdline::~Cmdline()
{
    if (m_thread.joinable())
        m_thread.join();
}

void Cmdline::run()
{
    std::string input;

    while (std::getline(std::cin, input))
    {
        if (input == "q")
        {
            m_queue.put(Quit{});
            break;
        }

        if (input == "s")
            m_queue.put(Stop{});

        if (input == "")
            m_queue.put(Toggle{});
    }
}

} // namespace app
