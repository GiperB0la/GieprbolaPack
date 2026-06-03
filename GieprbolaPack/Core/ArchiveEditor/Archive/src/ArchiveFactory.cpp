#include "../include/ArchiveFactory.hpp"

#include "../../Zip/include/ZipWriter.hpp"
// #include "../../Zip/include/ZipReader.hpp"

std::unique_ptr<IArchiveWriter> ArchiveFactory::create_writer(ArchiveType type)
{
    switch (type) {
    case ArchiveType::Zip:
        return std::make_unique<ZipWriter>();

    case ArchiveType::Rar:
        return nullptr;
    }

    return nullptr;
}

std::unique_ptr<IArchiveReader> ArchiveFactory::create_reader(ArchiveType type)
{
    switch (type) {
    case ArchiveType::Zip:
        // return std::make_unique<ZipReader>();
        return nullptr;

    case ArchiveType::Rar:
        return nullptr;
    }

    return nullptr;
}