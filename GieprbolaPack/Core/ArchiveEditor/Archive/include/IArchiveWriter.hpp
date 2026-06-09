#pragma once
#include <filesystem>
#include <vector>

#include "ArchiveTypes.hpp"

class IArchiveWriter
{
public:
    virtual ~IArchiveWriter() = default;

public:
    virtual void create(const std::filesystem::path& archive_path, const std::vector<ArchiveFile>& files, CompressionMode compression_mode) = 0;
};