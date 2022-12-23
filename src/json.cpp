#include <asx/json.hpp>

#include <bln_net/utils.hpp>

#include <msgpack.hpp>

namespace asx {

auto to_string(const json& j, const int indent) -> std::string
{
    return j.dump(indent);
}

auto from_string(const std::string& s) -> json
{
    return json::parse(s);
}

auto to_bytes(const json& j) -> bln_net::bytes
{
    return json::to_msgpack(j);
}

auto from_bytes(const bln_net::bytes& b) -> json
{
    return json::from_msgpack(b);
}

} // namespace asx
