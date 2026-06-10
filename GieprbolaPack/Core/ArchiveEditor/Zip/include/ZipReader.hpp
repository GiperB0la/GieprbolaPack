#pragma once
#include <fstream>
#include <vector>
#include <filesystem>
#include <zlib.h>

#include "../../../Checksum/include/Checksum.hpp"
#include "../../Archive/include/ArchiveTypes.hpp"
#include "../../Archive/include/IArchiveReader.hpp"

class ZipReader : public IArchiveReader
{
public:
    void open(const std::filesystem::path& archive_path) override;
    void extract_all(const std::filesystem::path& destination) override;
    void extract_file(const std::string& archive_name, const std::filesystem::path& destination) override;

    std::vector<ArchiveFile> files() const override;

private:
    struct Entry {
        std::string archive_name;

        uint32_t crc32 = 0;

        uint32_t compressed_size = 0;
        uint32_t uncompressed_size = 0;

        uint32_t local_header_offset = 0;

        uint16_t compression_method = 0;

        uint16_t mod_time = 0;
        uint16_t mod_date = 0;
    };

private:
    void read_end_of_central_directory();
    void read_central_directory();
    void read_central_directory_header();

    std::vector<char> read_file_data(const Entry& entry);
    std::vector<char> inflate_data(const std::vector<char>& compressed, uint32_t uncompressed_size);

    uint16_t read_u16();
    uint32_t read_u32();

private:
    std::ifstream input_;

    uint32_t central_directory_offset_ = 0;
    uint32_t central_directory_size_ = 0;

    std::vector<Entry> entries_;
};