//
// Operations
//

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace integrityd
{
void check_file(const fs::path &file);
}
