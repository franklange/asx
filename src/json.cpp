#include <asx/json.hpp>

#include <bln_net/utils.hpp>

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
    return bln_net::to_bytes(to_string(j));
}

auto from_bytes(const bln_net::bytes& b) -> json
{
    return from_string(bln_net::to_string(b));
}

} // namespace asx
