#pragma once

namespace asx {

struct Sink
{
    virtual ~Sink() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
};

} // namespace asx
