#include "../include/ZipWriter.hpp"


ZipWriter& ZipWriter::instance()
{
    static ZipWriter writer;
    return writer;
}

void ZipWriter::create_archive(const std::filesystem::path& archive_path, const std::vector<std::filesystem::path>& files, CompressionMode mode)
{
    entries_.clear();

    output_.open(
        archive_path,
        std::ios::binary | std::ios::trunc
    );

    if (!output_.is_open()) {
        throw std::runtime_error("Failed to create archive");
    }

    for (const auto& file : files) {
        add_file(file, mode);
    }

    const uint32_t central_directory_offset = current_offset();

    for (const auto& entry : entries_) {
        write_central_directory_header(entry, mode);
    }

    const uint32_t central_directory_size =
        current_offset() - central_directory_offset;

    write_end_of_central_directory(
        central_directory_offset,
        central_directory_size
    );

    output_.close();
}

void ZipWriter::add_file(const std::filesystem::path& file_path, CompressionMode mode)
{
    if (!output_.is_open()) {
        throw std::runtime_error("ZIP output file is not open");
    }

    if (mode != CompressionMode::Store) {
        throw std::runtime_error("Only Store compression mode is supported now");
    }

    if (!std::filesystem::is_regular_file(file_path)) {
        throw std::runtime_error("ZIP entry is not a regular file");
    }

    const auto file_size = std::filesystem::file_size(file_path);

    if (file_size > UINT32_MAX) {
        throw std::runtime_error("File is too large for simple ZIP writer");
    }

    Entry entry;

    entry.archive_name = make_archive_name(file_path);
    entry.crc32 = Checksum::instance().crc32_file(file_path);
    entry.compressed_size = static_cast<uint32_t>(file_size);
    entry.uncompressed_size = static_cast<uint32_t>(file_size);
    entry.local_header_offset = current_offset();

    make_dos_time_date(file_path, entry.mod_time, entry.mod_date);

    write_local_file_header(entry, mode);
    copy_file_data(file_path);

    entries_.push_back(entry);
}

void ZipWriter::write_local_file_header(const Entry& entry, CompressionMode mode)
{
    constexpr uint16_t version_needed = 20;
    constexpr uint16_t general_purpose_flag = 0x0800; // UTF-8 names

    write_u32(0x04034b50);

    write_u16(version_needed);
    write_u16(general_purpose_flag);
    write_u16(compression_method(mode));

    write_u16(entry.mod_time);
    write_u16(entry.mod_date);

    write_u32(entry.crc32);
    write_u32(entry.compressed_size);
    write_u32(entry.uncompressed_size);

    write_u16(static_cast<uint16_t>(entry.archive_name.size()));
    write_u16(0);

    output_.write(
        entry.archive_name.data(),
        static_cast<std::streamsize>(entry.archive_name.size())
    );
}

void ZipWriter::write_central_directory_header(const Entry& entry, CompressionMode mode)
{
    constexpr uint16_t version_made_by = 20;
    constexpr uint16_t version_needed = 20;
    constexpr uint16_t general_purpose_flag = 0x0800; // UTF-8 names

    write_u32(0x02014b50);

    write_u16(version_made_by);
    write_u16(version_needed);
    write_u16(general_purpose_flag);
    write_u16(compression_method(mode));

    write_u16(entry.mod_time);
    write_u16(entry.mod_date);

    write_u32(entry.crc32);
    write_u32(entry.compressed_size);
    write_u32(entry.uncompressed_size);

    write_u16(static_cast<uint16_t>(entry.archive_name.size()));
    write_u16(0);
    write_u16(0);

    write_u16(0);
    write_u16(0);
    write_u32(0);

    write_u32(entry.local_header_offset);

    output_.write(
        entry.archive_name.data(),
        static_cast<std::streamsize>(entry.archive_name.size())
    );
}

void ZipWriter::write_end_of_central_directory(uint32_t central_directory_offset, uint32_t central_directory_size)
{
    if (entries_.size() > UINT16_MAX) {
        throw std::runtime_error("Too many files for simple ZIP writer");
    }

    const uint16_t entries_count =
        static_cast<uint16_t>(entries_.size());

    write_u32(0x06054b50);

    write_u16(0);
    write_u16(0);

    write_u16(entries_count);
    write_u16(entries_count);

    write_u32(central_directory_size);
    write_u32(central_directory_offset);

    write_u16(0);
}

void ZipWriter::copy_file_data(const std::filesystem::path& file_path)
{
    std::ifstream input(file_path, std::ios::binary);

    if (!input.is_open()) {
        throw std::runtime_error("Failed to open file for ZIP writing");
    }

    std::array<char, 8192> buffer{};

    while (input) {
        input.read(buffer.data(), buffer.size());

        const std::streamsize bytes_read = input.gcount();

        if (bytes_read > 0) {
            output_.write(buffer.data(), bytes_read);
        }
    }
}

uint32_t ZipWriter::current_offset()
{
    const auto position = output_.tellp();

    if (position < 0) {
        throw std::runtime_error("Failed to get ZIP output offset");
    }

    if (static_cast<uint64_t>(position) > UINT32_MAX) {
        throw std::runtime_error("ZIP file is too large for simple ZIP writer");
    }

    return static_cast<uint32_t>(position);
}

void ZipWriter::write_u16(uint16_t value)
{
    output_.put(static_cast<char>(value & 0xFF));
    output_.put(static_cast<char>((value >> 8) & 0xFF));
}

void ZipWriter::write_u32(uint32_t value)
{
    output_.put(static_cast<char>(value & 0xFF));
    output_.put(static_cast<char>((value >> 8) & 0xFF));
    output_.put(static_cast<char>((value >> 16) & 0xFF));
    output_.put(static_cast<char>((value >> 24) & 0xFF));
}

uint16_t ZipWriter::compression_method(CompressionMode mode)
{
    switch (mode) {
    case CompressionMode::Store:
        return 0;

    case CompressionMode::Deflate:
        return 8;

    case CompressionMode::Bzip2:
        return 12;

    case CompressionMode::Lzma:
        return 14;
    }

    return 0;
}

std::string ZipWriter::make_archive_name(const std::filesystem::path& file_path)
{
    const auto u8_name = file_path.filename().u8string();

    std::string name(
        reinterpret_cast<const char*>(u8_name.data()),
        u8_name.size()
    );

    for (char& ch : name) {
        if (ch == '\\') {
            ch = '/';
        }
    }

    return name;
}

void ZipWriter::make_dos_time_date(const std::filesystem::path& file_path, uint16_t& dos_time, uint16_t& dos_date)
{
    const auto file_time = std::filesystem::last_write_time(file_path);
    const auto system_time = std::chrono::clock_cast<std::chrono::system_clock>(file_time);

    const std::time_t time = std::chrono::system_clock::to_time_t(system_time);

    std::tm local_time{};

#ifdef _WIN32
    localtime_s(&local_time, &time);
#else
    localtime_r(&time, &local_time);
#endif

    dos_time =
        static_cast<uint16_t>(
            (local_time.tm_hour << 11) |
            (local_time.tm_min << 5) |
            (local_time.tm_sec / 2)
            );

    dos_date =
        static_cast<uint16_t>(
            ((local_time.tm_year + 1900 - 1980) << 9) |
            ((local_time.tm_mon + 1) << 5) |
            local_time.tm_mday
            );
}