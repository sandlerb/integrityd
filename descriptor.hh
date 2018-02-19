//
// Integrity Descriptor
//

#pragma once

#include "checksum.hh"

#include <experimental/filesystem>
#include <string>

namespace fs = std::experimental::filesystem;


namespace integrityd
{

bool descriptor_exists(fs::path target);
fs::path descriptor_path(fs::path target);

struct target_metadata
{
    fs::path target;
    size_t target_size_bytes = 0;

    bool operator==(const target_metadata &other) const
    {
        return (target == other.target) && (target_size_bytes == other.target_size_bytes);
    }

    // TODO last modified time
};

class descriptor
{
public:
    // Only allow creation from a target or an on-disk descriptor
    descriptor() = delete;

    //
    // Create a descriptor from a target
    //
    descriptor(const fs::path &target, const checksum_t cst)
    {
        // TODO check if target exists
        //metadata_ = metadata_from_target(target);
        checksum_type_ = cst;
        checksum_ = compute();
    }

    //
    // Create an in-memory descriptor from an existing descriptor on disk
    //
    //static const descriptor load(const fs::path &path)
    //{
    //}

    //
    // Write the descriptor to disk
    //
    void write(const fs::path &path);

    //
    // Operator overloads
    //
    bool operator==(const descriptor &other) const
    {
        return (metadata_ == other.metadata())
               && (checksum_ == other.checksum())
               && (checksum_type_ == other.checksum_type());
    }

    bool operator!=(const descriptor &other) const
    {
        return !(*this == other);
    }

    //
    // Accessors
    //
    target_metadata metadata() const;
    std::string checksum() const;
    checksum_t checksum_type() const;

    //
    // The descriptor is tied to a specific file, do not allow copies.  Move is okay.
    //
    descriptor(const descriptor&) = delete;
    descriptor& operator=(const descriptor&) = delete;

private:
    //
    // Compute the checksum for the target of the descriptor
    //
    std::string compute()
    {
        return checksum_factory(checksum_type_)(metadata_.target);
    }

    //target_metadata metadata_from_target(const fs::path &path);

    target_metadata metadata_;
    std::string checksum_;
    checksum_t checksum_type_;
};
}
