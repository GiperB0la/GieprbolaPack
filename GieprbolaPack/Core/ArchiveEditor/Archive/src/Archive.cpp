#include "../include/Archive.hpp"


Archive& Archive::instance()
{
    static Archive archive;
    return archive;
}

void Archive::pack(const std::filesystem::path& input_path, const std::filesystem::path& archive_path)
{
    const auto files = FileScanner::instance().scan(input_path);

    std::ofstream archive(archive_path, std::ios::binary);

    if (!archive.is_open()) {
        throw std::runtime_error("Failed to create archive");
    }

    ArchiveHeader header{};
    std::memcpy(header.magic, "GPACK01", 8);
    header.version = 1;
    header.file_count = static_cast<uint32_t>(files.size());

    archive.write(reinterpret_cast<const char*>(&header), sizeof(header));

    std::array<char, 8192> buffer{};

    for (const auto& file : files) {
        const std::string name = file.relative_path.generic_string();

        FileEntry entry{};
        entry.name_size = name.size();
        entry.original_size = file.size;
        entry.compressed_size = file.size;
        entry.crc32 = file.is_directory ? 0 : Checksum::instance().crc32_file(file.full_path);
        entry.is_directory = file.is_directory ? 1 : 0;

        archive.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
        archive.write(name.data(), static_cast<std::streamsize>(name.size()));

        if (file.is_directory) {
            continue;
        }

        std::ifstream input(file.full_path, std::ios::binary);

        if (!input.is_open()) {
            throw std::runtime_error("Failed to open input file");
        }

        while (input) {
            input.read(buffer.data(), buffer.size());
            const std::streamsize bytes_read = input.gcount();

            if (bytes_read > 0) {
                archive.write(buffer.data(), bytes_read);
            }
        }
    }
}

void Archive::unpack(const std::filesystem::path& archive_path, const std::filesystem::path& output_path)
{
    std::ifstream archive(archive_path, std::ios::binary);

    if (!archive.is_open()) {
        throw std::runtime_error("Failed to open archive");
    }

    ArchiveHeader header{};
    archive.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (std::memcmp(header.magic, "GPACK01", 8) != 0) {
        throw std::runtime_error("Invalid archive format");
    }

    std::filesystem::create_directories(output_path);

    std::array<char, 8192> buffer{};

    for (uint32_t i = 0; i < header.file_count; ++i) {
        FileEntry entry{};
        archive.read(reinterpret_cast<char*>(&entry), sizeof(entry));

        std::string name(entry.name_size, '\0');
        archive.read(name.data(), static_cast<std::streamsize>(entry.name_size));

        const auto target_path = output_path / std::filesystem::path(name);

        if (entry.is_directory) {
            std::filesystem::create_directories(target_path);
            continue;
        }

        std::filesystem::create_directories(target_path.parent_path());

        std::ofstream output(target_path, std::ios::binary);

        if (!output.is_open()) {
            throw std::runtime_error("Failed to create output file");
        }

        uint64_t remaining = entry.compressed_size;

        while (remaining > 0) {
            const auto chunk_size = static_cast<std::streamsize>(
                std::min<uint64_t>(buffer.size(), remaining)
                );

            archive.read(buffer.data(), chunk_size);
            const std::streamsize bytes_read = archive.gcount();

            if (bytes_read <= 0) {
                throw std::runtime_error("Unexpected end of archive");
            }

            output.write(buffer.data(), bytes_read);
            remaining -= static_cast<uint64_t>(bytes_read);
        }

        output.close();

        const uint32_t crc = Checksum::instance().crc32_file(target_path);

        if (crc != entry.crc32) {
            throw std::runtime_error("CRC32 mismatch: " + name);
        }
    }
}