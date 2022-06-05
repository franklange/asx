#pragma once

#include <asx/queue.hpp>

#include <thread>

namespace app {

using namespace asx;

class Cmdline
{
public:
    Cmdline(ActionQueue&);
    ~Cmdline();

private:
    void run();

    ActionQueue& m_queue;
    std::thread m_thread;
};

} // namespace app
