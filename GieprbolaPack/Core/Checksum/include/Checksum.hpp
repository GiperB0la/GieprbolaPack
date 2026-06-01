#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>

class Checksum
{
public:
    static Checksum& instance();

private:
    Checksum();
    Checksum(const Checksum&) = delete;
    Checksum& operator=(const Checksum&) = delete;

public:
    uint32_t crc32(const std::vector<uint8_t>& data) const;
    uint32_t crc32(const uint8_t* data, std::size_t size) const;
    uint32_t crc32_file(const std::filesystem::path& file_path) const;

private:
    std::array<uint32_t, 256> table_;
};