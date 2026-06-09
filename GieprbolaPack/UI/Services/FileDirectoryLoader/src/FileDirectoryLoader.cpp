#include "../include/FileDirectoryLoader.hpp"


DirectoryLoadResult FileDirectoryLoader::load(const QString& path) const
{
    DirectoryLoadResult result;

	std::filesystem::path fs_path(path.toStdWString());

    if (!std::filesystem::exists(fs_path)) {
        result.error_text = QObject::tr("Path not found");
        return result;
    }

    if (!std::filesystem::is_directory(fs_path)) {
        result.error_text = QObject::tr("It's not a folder");
        return result;
    }

    result.files = FileScanner::instance().scan(fs_path);
    result.success = true;

    return result;
}