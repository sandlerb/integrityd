//
// Integrity Descriptor
//

#include "file_chunker.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <vector>

static bool compare(const file_chunker &fv, const std::vector<uint8_t> data)
{
    auto it = data.begin();
    for (auto const &chunk : fv)
    {
        // TODO might throw
        auto const &slice = std::vector<uint8_t>(it, it + chunk.data->size());
        
        if (slice != *(chunk.data))
        {
            return false;
        }

        it += chunk.data->size();
    }

    return true;
}

static void write_vector_to_disk(const std::vector<uint8_t> &vec, const std::string &path)
{
    std::ofstream out_file(path);
    std::ostream_iterator<uint8_t> out_it(out_file);
    std::copy(vec.begin(), vec.end(), out_it);
}

static void randomize_vector(std::vector<uint8_t> &vec)
{
    static std::default_random_engine generator;
	static std::uniform_int_distribution<uint8_t> distribution(
            std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max());
    std::generate(vec.begin(), vec.end(), []() { return distribution(generator); });
}

static bool test_even_sized_file()
{
    auto const file_path = "/tmp/fvt_even";

    std::vector<uint8_t> data;
    randomize_vector(data);
    
    write_vector_to_disk(data, file_path);
    file_chunker fv(file_path, 256);

    auto const result = compare(fv, data);

    std::remove(file_path);

    return result;
}

static bool test_file_with_remainder()
{
    auto const file_path = "/tmp/fvt_remainder";

    std::vector<uint8_t> data;
    randomize_vector(data);

    write_vector_to_disk(data, file_path);
    file_chunker fv(file_path, 256);

    auto const result = compare(fv, data);

    std::remove(file_path);

    return result;
}

static void print_result(const std::string test, const bool result)
{
    std::cout << test << ": " << (result ? "PASS" : "FAIL") << std::endl;
}

int main()
{
    print_result("Even sized file", test_even_sized_file());
    print_result("File with remainder", test_file_with_remainder());
}
