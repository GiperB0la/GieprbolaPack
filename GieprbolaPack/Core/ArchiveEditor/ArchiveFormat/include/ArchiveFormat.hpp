#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct ArchiveHeader 
{
    char magic[8];              // "GPACK01"
    uint32_t version;
    uint32_t file_count;
};

struct FileEntry 
{
    uint64_t name_size;
    uint64_t original_size;
    uint64_t compressed_size;   // пока = original_size
    uint32_t crc32;
    uint8_t is_directory;
};

#pragma pack(pop)