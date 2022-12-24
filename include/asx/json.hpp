#pragma once

#include <bln_net/types.hpp>

#include <nlohmann/json.hpp>

#include <string>

namespace asx {

using json = nlohmann::json;

auto to_string(const json&, int indent = -1) -> std::string;

auto to_bytes(const json&) -> bln_net::bytes;
auto from_bytes(const bln_net::bytes&) -> json;

} // namespace asx
