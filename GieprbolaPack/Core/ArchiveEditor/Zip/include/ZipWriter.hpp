#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <chrono>
#include <stdexcept>

#include "../../../Checksum/include/Checksum.hpp"
#include "../../Archive/include/ArchiveTypes.hpp"
#include "../../Archive/include/IArchiveWriter.hpp"

class ZipWriter : public IArchiveWriter
{
public:
    void create(const std::filesystem::path& archive_path, const std::vector<std::filesystem::path>& files, CompressionMode mode) override;

private:
    struct Entry {
        std::string archive_name;

        uint32_t crc32 = 0;

        uint32_t compressed_size = 0;
        uint32_t uncompressed_size = 0;

        uint32_t local_header_offset = 0;

        uint16_t mod_time = 0;
        uint16_t mod_date = 0;
    };

private:
    void add_file(const std::filesystem::path& file_path, CompressionMode mode);
    void write_local_file_header(const Entry& entry, CompressionMode mode);
    void write_central_directory_header(const Entry& entry, CompressionMode mode);
    void write_end_of_central_directory(uint32_t central_directory_offset, uint32_t central_directory_size);
    void copy_file_data(const std::filesystem::path& file_path);

    uint32_t current_offset();
    void write_u16(uint16_t value);
    void write_u32(uint32_t value);

private:
    static uint16_t compression_method(CompressionMode mode);
    static std::string make_archive_name(const std::filesystem::path& file_path);
    static void make_dos_time_date(const std::filesystem::path& file_path, uint16_t& dos_time, uint16_t& dos_date);

private:
    std::ofstream output_;
    std::vector<Entry> entries_;
};