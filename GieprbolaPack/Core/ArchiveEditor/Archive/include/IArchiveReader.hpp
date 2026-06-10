#pragma once
#include <filesystem>
#include <vector>

class IArchiveReader
{
public:
    virtual ~IArchiveReader() = default;

public:
    virtual void open(const std::filesystem::path& archive_path) = 0;
    virtual void extract_all(const std::filesystem::path& destination) = 0;
    virtual void extract_file(const std::string& archive_name, const std::filesystem::path& destination) = 0;
    virtual std::vector<ArchiveFile> files() const = 0;
};