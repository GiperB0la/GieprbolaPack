#pragma once
#include <vector>
#include <filesystem>

enum class ArchiveType 
{
    Zip,
    Rar
};

enum class CompressionMode
{
    Deflate,     // ZIP Deflate
    Store      // без сжатия
};

enum class ArchiveOpenMode 
{
    Read,
    Write
};

struct ArchiveFile
{
    std::filesystem::path source_path;
    std::string archive_name;

    ArchiveFile(const std::filesystem::path& source_path, const std::string& archive_name)
        : source_path(source_path)
        , archive_name(archive_name)
    {
    }
};