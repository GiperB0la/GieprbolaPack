#include "../include/ZipReader.hpp"


void ZipReader::open(const std::filesystem::path& archive_path)
{
    entries_.clear();

    input_.open(archive_path, std::ios::binary);

    if (!input_.is_open()) {
        throw std::runtime_error("Failed to open ZIP archive");
    }

    read_end_of_central_directory();
    read_central_directory();
}

void ZipReader::extract_file(const std::string& archive_name, const std::filesystem::path& destination) {
    auto it =
        std::find_if(
            entries_.begin(),
            entries_.end(),
            [&](const Entry& entry)
            {
                return entry.archive_name == archive_name;
            });

    if (it == entries_.end()) {
        throw std::runtime_error("File not found in archive");
    }

    const std::vector<char> data =
        read_file_data(*it);

    std::filesystem::path output_path =
        destination / archive_name;

    std::filesystem::create_directories(
        output_path.parent_path());

    std::ofstream output(
        output_path,
        std::ios::binary);

    if (!output.is_open()) {
        throw std::runtime_error("Failed to create output file");
    }

    if (!data.empty()) {
        output.write(
            data.data(),
            static_cast<std::streamsize>(data.size()));
    }
}

void ZipReader::extract_all(const std::filesystem::path& destination)
{
    for (const Entry& entry : entries_) {
        extract_file(
            entry.archive_name,
            destination);
    }
}

std::vector<ArchiveFile> ZipReader::files() const
{
    std::vector<ArchiveFile> result;

    result.reserve(entries_.size());

    for (const Entry& entry : entries_) {
        result.emplace_back(
            std::filesystem::path{},
            entry.archive_name);
    }

    return result;
}

void ZipReader::read_end_of_central_directory()
{
    input_.seekg(0, std::ios::end);

    const std::streamoff file_size = input_.tellg();

    constexpr size_t max_comment_size = 65535;
    const size_t search_size =
        static_cast<size_t>(
            std::min<std::streamoff>(
                file_size,
                max_comment_size + 22
            )
            );

    std::vector<char> buffer(search_size);

    input_.seekg(file_size - static_cast<std::streamoff>(search_size));
    input_.read(buffer.data(), search_size);

    for (int i = static_cast<int>(search_size) - 22; i >= 0; --i) {
        uint32_t signature =
            static_cast<uint8_t>(buffer[i]) |
            (static_cast<uint8_t>(buffer[i + 1]) << 8) |
            (static_cast<uint8_t>(buffer[i + 2]) << 16) |
            (static_cast<uint8_t>(buffer[i + 3]) << 24);

        if (signature == 0x06054b50) {

            const uint8_t* ptr =
                reinterpret_cast<const uint8_t*>(buffer.data() + i);

            central_directory_size_ =
                ptr[12] |
                (ptr[13] << 8) |
                (ptr[14] << 16) |
                (ptr[15] << 24);

            central_directory_offset_ =
                ptr[16] |
                (ptr[17] << 8) |
                (ptr[18] << 16) |
                (ptr[19] << 24);

            return;
        }
    }

    throw std::runtime_error("End of central directory not found");
}

void ZipReader::read_central_directory()
{
    input_.seekg(central_directory_offset_);

    while (true) {

        uint32_t signature = read_u32();

        if (signature != 0x02014b50) {
            break;
        }

        Entry entry;

        read_u16(); // version made by
        read_u16(); // version needed
        read_u16(); // flags

        entry.compression_method = read_u16();

        entry.mod_time = read_u16();
        entry.mod_date = read_u16();

        entry.crc32 = read_u32();

        entry.compressed_size = read_u32();
        entry.uncompressed_size = read_u32();

        uint16_t name_length = read_u16();
        uint16_t extra_length = read_u16();
        uint16_t comment_length = read_u16();

        read_u16();
        read_u16();
        read_u32();

        entry.local_header_offset = read_u32();

        std::string name(name_length, '\0');
        input_.read(name.data(), name_length);

        entry.archive_name = std::move(name);

        input_.seekg(extra_length + comment_length, std::ios::cur);

        entries_.push_back(std::move(entry));
    }
}

std::vector<char> ZipReader::read_file_data(const Entry& entry)
{
    input_.seekg(entry.local_header_offset);

    if (read_u32() != 0x04034b50) {
        throw std::runtime_error("Bad local header");
    }

    read_u16();
    read_u16();
    read_u16();
    read_u16();
    read_u16();

    read_u32();
    read_u32();
    read_u32();

    uint16_t name_length = read_u16();
    uint16_t extra_length = read_u16();

    input_.seekg(name_length + extra_length, std::ios::cur);

    std::vector<char> compressed(entry.compressed_size);

    input_.read(
        compressed.data(),
        static_cast<std::streamsize>(compressed.size())
    );

    if (entry.compression_method == 0) {
        return compressed;
    }

    if (entry.compression_method == 8) {
        return inflate_data(compressed, entry.uncompressed_size);
    }

    throw std::runtime_error("Unsupported compression method");
}

std::vector<char> ZipReader::inflate_data(const std::vector<char>& compressed, uint32_t uncompressed_size)
{
    z_stream stream{};

    const int init_result =
        inflateInit2(&stream, -MAX_WBITS);

    if (init_result != Z_OK) {
        throw std::runtime_error("Failed to initialize inflate");
    }

    std::vector<char> output(uncompressed_size);

    stream.next_in =
        reinterpret_cast<Bytef*>(const_cast<char*>(compressed.data()));

    stream.avail_in =
        static_cast<uInt>(compressed.size());

    stream.next_out =
        reinterpret_cast<Bytef*>(output.data());

    stream.avail_out =
        static_cast<uInt>(output.size());

    const int result =
        inflate(&stream, Z_FINISH);

    inflateEnd(&stream);

    if (result != Z_STREAM_END) {
        throw std::runtime_error("Inflate failed");
    }

    output.resize(stream.total_out);

    return output;
}

uint16_t ZipReader::read_u16()
{
    const uint8_t b0 =
        static_cast<uint8_t>(input_.get());

    const uint8_t b1 =
        static_cast<uint8_t>(input_.get());

    return
        static_cast<uint16_t>(
            b0 |
            (b1 << 8));
}

uint32_t ZipReader::read_u32()
{
    const uint8_t b0 =
        static_cast<uint8_t>(input_.get());

    const uint8_t b1 =
        static_cast<uint8_t>(input_.get());

    const uint8_t b2 =
        static_cast<uint8_t>(input_.get());

    const uint8_t b3 =
        static_cast<uint8_t>(input_.get());

    return
        static_cast<uint32_t>(
            b0 |
            (b1 << 8) |
            (b2 << 16) |
            (b3 << 24));
}