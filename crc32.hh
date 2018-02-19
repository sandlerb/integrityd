//
// Integrity Descriptor
//

#pragma once

#include <array>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::experimental::filesystem;

namespace integrityd
{
namespace crc32
{
void init_table();
std::string calculate(const fs::path &target, size_t chunk_size);
}
}
