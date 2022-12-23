#include <asx/protocol.hpp>

#include <bln_net/utils.hpp>

#include <algorithm>

namespace asx {

namespace {

auto is_track(const json& j) -> bool
{
    return j.contains("id")   && j["id"].is_number()
        && j.contains("name") && j["name"].is_string()
        && j.contains("size") && j["size"].is_number();
}

auto is_segment(const json& j) -> bool
{
    return j.contains("id")  && j["id"].is_number()
        && j.contains("seg") && j["seg"].is_number();
}

} // namespace anonym

auto parse_req_tracks(const json& j) -> std::optional<req_tracks>
{
    return req_tracks{};
}

auto parse(rep_tracks& r, const json& j) -> bool
{
    for (const auto& t : j)
        if (!is_track(t)) return false;

    r.tracks = j;
    return true;
}

auto parse_req_segment(const json& j) -> std::optional<req_segment>
{
    if (!is_segment(j))
        return {};

    req_segment req;

    req.track_id   = j["id"];
    req.segment_id = j["seg"];

    return req;
}

auto parse(rep_segment& r, const json& j) -> bool
{
    if (!is_segment(j))
        return false;

    r.track_id = j["id"];
    r.segment_id = j["seg"];

    auto& b = j["data"];
    auto* d = reinterpret_cast<bytes::value_type*>(r.data.data());
    std::copy(b.begin(), b.end(), d);

    return true;
}

auto make_req_tracks() -> bytes
{
    return to_bytes(json{
        {"head", req_tracks::name},
        {"body", {}},
    });
}

auto make_rep_tracks(json tracks) -> bytes
{
    return to_bytes(json{
        {"head", rep_tracks::name},
        {"body", std::move(tracks)}
    });
}

auto make_req_segment(const u16 tid, const u16 sid) -> bytes
{
    return to_bytes(json{
        {"head", req_segment::name},
        {"body", {
            {"id", tid},
            {"seg", sid}
        }}
    });
}

auto make_rep_segment(const u16 tid, const u16 sid, const segment& d) -> bytes
{
    auto *b = reinterpret_cast<const bytes::value_type*>(d.data());

    return to_bytes(json{
        {"head", rep_segment::name},
        {"body", {
            {"id", tid},
            {"seg", sid},
            {"data", bytes{b, b + sizeof(segment)}},
        }}
    });
}

} // namespace asx
