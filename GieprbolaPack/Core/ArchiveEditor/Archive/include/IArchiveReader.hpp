#pragma once
#include <filesystem>
#include <vector>

class IArchiveReader
{
public:
    virtual ~IArchiveReader() = default;

public:
    virtual void open(const std::filesystem::path& archive_path) = 0;
    virtual void extract_to(const std::filesystem::path& output_path) = 0;
};