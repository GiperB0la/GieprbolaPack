#include "../include/FileScanner.hpp"


FileScanner& FileScanner::instance()
{
    static FileScanner scanner;
    return scanner;
}

FileScanner::FileScanner()
    : root_path_(std::filesystem::current_path())
{

}

std::vector<FileInfo> FileScanner::scan(const std::filesystem::path& root_path)
{
    if (!std::filesystem::exists(root_path)) {
        throw std::runtime_error("Path does not exist");
    }

    std::vector<FileInfo> result;

    const auto absolute_root = std::filesystem::absolute(root_path);

    if (std::filesystem::is_regular_file(absolute_root)) {
        FileInfo info;
        info.full_path = absolute_root;
        info.relative_path = absolute_root.filename();
        info.size = std::filesystem::file_size(absolute_root);
        info.is_directory = false;
        info.last_write_time = std::filesystem::last_write_time(absolute_root);

        result.push_back(info);
        return result;
    }

    if (!std::filesystem::is_directory(absolute_root)) {
        throw std::runtime_error("Path is not a file or directory");
    }

    for (const auto& entry : std::filesystem::directory_iterator(absolute_root)) {
        FileInfo info;

        info.full_path = entry.path();
        info.relative_path = entry.path().filename();
        info.is_directory = entry.is_directory();
		info.last_write_time = std::filesystem::last_write_time(entry.path());

        if (entry.is_regular_file()) {
            info.size = entry.file_size();
        }

        result.push_back(info);
    }

	files_ = result;

    return files_;
}