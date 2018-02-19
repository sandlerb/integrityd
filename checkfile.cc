#include "crc32.hh"

#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

const int ARG_COUNT = 3;

namespace
{
void print_usage()
{
    std::cerr << "Usage: <checksum_type> <file>" << std::endl;
    std::cerr << "Supported checksum types: crc32" << std::endl;
}
}

int main(int argc, const char** argv)
{
    if (argc != ARG_COUNT)
    {
        print_usage();

        return 1;
    }

    integrityd::crc32::init_table();

    const fs::path target(argv[2]);

    const auto checksum = integrityd::crc32::calculate(target, 4096 * 128);

    std::cout << checksum << std::endl;

    return 0;
}
