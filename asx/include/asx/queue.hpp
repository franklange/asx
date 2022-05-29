#pragma once

#include <asx/audio_settings.hpp>

#include <bln/queue/data_queue.hpp>
#include <bln/queue/msg_queue.hpp>

namespace asx {

using AudioQueue = bln::queue::DataQueue<Buffer, 256>;
using Semaphore = bln::queue::MsgQueue<bool>;

} // namespace asx
