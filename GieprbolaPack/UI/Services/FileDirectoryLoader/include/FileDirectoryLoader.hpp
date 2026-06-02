#pragma once
#include <filesystem>
#include <vector>

#include <QObject>
#include <QString>

#include "../../../../Core/FileScanner/include/FileScanner.hpp"

struct DirectoryLoadResult
{
    bool success = false;
    QString error_text;
    std::vector<FileInfo> files;
};

class FileDirectoryLoader
{
public:
    DirectoryLoadResult load(const QString& path) const;
};