//
// Operations
//

#include <experimental/filesystem>
#include <iostream>
#include <vector>

namespace fs = std::experimental::filesystem;

namespace integrityd
{

void handle_corruption(const fs::path &file)
{
    std::cout << file << " is corrupted!" << std::endl;
}

void check_file(const fs::path &file)
{
    if (id::descriptor_exists(file) == false)
    {
        // It's a new file, just generate a descriptor
        id::descriptor d(file);
        d.write();
    }
    else
    {
        id::descriptor previous = id::descriptor::from_descriptor(id::descriptor_path(file));
        id::descriptor current = id::descriptor::from_target(file);

        // The file has changed, go with the new descriptor
        if (previous.mtime() != current.mtime())
        {
            current.write();
        }
        // We have a previous integrity file, make sure things are still okay
        else if (previous != current)
        {
            // Uh oh, data corruption, immediately cache this for a future report
            handle_corruption(file);
        }
    }
}
}
