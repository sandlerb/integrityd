//
// Daemon
//

#include "operations.hh"

#include <iostream>
#include <string>
#include <vector>

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


int main(int argc, char** argv)
{
    // TODO need a logger
    std::cout << "Starting integrityd" << std::endl;

    std::vector<fs::directory_entry> directories;

    // Flow
    //  1 Load default config file
    //  2 Allow configurable config file path
    //  3 Get directories from config
    //  4 Use recursive directory iterator, check everything

    for (size_t i = 1; i < static_cast<size_t>(argc); ++i)
    {
        fs::path p(argv[i]);
        if (fs::is_directory(p))
        {
            directories.emplace_back(fs::absolute(p));
        }
        else
        {
            std::cerr << p << " is not a directory!" << std::endl;
        }
    }

    for (const auto &d : directories)
    {
        for (const auto &p : fs::recursive_directory_iterator(d))
        {
            if (!fs::is_directory(p))
            {
                check_file(p);
            }
        }
    }

    return 0;
}
