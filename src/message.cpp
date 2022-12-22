#include <asx/message.hpp>

#include <cassert>
#include <sstream>

namespace asx {

auto pack_msg(std::string head, std::string body) -> std::string
{
    assert(head.size() > 0);
    assert(body.size() > 0);

    std::string res;
    res.reserve(head.size() + body.size() + 1);

    res += std::move(head);
    res += '|';
    res += std::move(body);

    return res;
}

auto unpack_msg(const std::string& m) -> message
{
    assert(m.size() > 0);

    message res;
    std::istringstream s{m};

    std::getline(s, res.head, '|');
    std::getline(s, res.body);

    assert(res.head.size() > 1);

    return res;
}

} // namespace asx
