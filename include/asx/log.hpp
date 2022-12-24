#pragma once

#include <bln_log/log.hpp>

#define asx_log(args, ...) bln_log_rel(args, ## __VA_ARGS__)
#define asx_dbg(args, ...) bln_log_dbg(args, ## __VA_ARGS__)
