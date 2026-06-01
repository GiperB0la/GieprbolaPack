#pragma once
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <array>

#include "../../ArchiveFormat/include/ArchiveFormat.hpp"
#include "../../../FileScanner/include/FileScanner.hpp"
#include "../../../Checksum/include/Checksum.hpp"

class Archive
{
public:
    static Archive& instance();

private:
    Archive() = default;
    Archive(const Archive&) = delete;
    Archive& operator=(const Archive&) = delete;

public:
    void pack(const std::filesystem::path& input_path, const std::filesystem::path& archive_path);
    void unpack(const std::filesystem::path& archive_path, const std::filesystem::path& output_path);
};