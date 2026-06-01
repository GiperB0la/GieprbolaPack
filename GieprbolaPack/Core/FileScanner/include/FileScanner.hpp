#pragma once
#include <filesystem>
#include <vector>
#include <stdexcept>

struct FileInfo
{
    std::filesystem::path full_path;
    std::filesystem::path relative_path;
    std::filesystem::file_time_type last_write_time;
    uint64_t size = 0;
    bool is_directory = false;
};

class FileScanner
{
public:
    static FileScanner& instance();

private:
    FileScanner();
    FileScanner(const FileScanner&) = delete;
    FileScanner& operator=(const FileScanner&) = delete;

public:
    std::vector<FileInfo> scan(const std::filesystem::path& root_path);
	std::filesystem::path get_root_path() const { return root_path_; }
	bool set_root_path(const std::filesystem::path& path) { root_path_ = path; return true; }

private:
	std::filesystem::path root_path_;
	std::vector<FileInfo> files_;
};