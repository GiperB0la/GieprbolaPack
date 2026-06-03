#pragma once
#include <memory>

#include "ArchiveTypes.hpp"
#include "IArchiveReader.hpp"
#include "IArchiveWriter.hpp"

class ArchiveFactory
{
public:
    static std::unique_ptr<IArchiveWriter> create_writer(ArchiveType type);
    static std::unique_ptr<IArchiveReader> create_reader(ArchiveType type);
};