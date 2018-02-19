//
// Integrity Descriptor
//

#pragma once

#include "crc32.hh"

#include <experimental/filesystem>
#include <functional>
#include <string>

namespace fs = std::experimental::filesystem;


namespace integrityd
{
using checksum_func = std::function<std::string(const fs::path &)>;

enum class checksum_t
{
    CRC32
};

const checksum_func checksum_factory(const checksum_t cst)
{
    switch(cst)
    {
    case checksum_t::CRC32:
        return crc32::calculate;
    }

    // TODO raise or assert false
    return crc32::calculate;
}
}
