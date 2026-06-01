#include "../include/Checksum.hpp"


Checksum& Checksum::instance()
{
    static Checksum checksum;
    return checksum;
}

Checksum::Checksum()
{
    constexpr uint32_t polynomial = 0xEDB88320;

    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;

        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            }
            else {
                crc >>= 1;
            }
        }

        table_[i] = crc;
    }
}

uint32_t Checksum::crc32(const std::vector<uint8_t>& data) const
{
    return crc32(data.data(), data.size());
}

uint32_t Checksum::crc32(const uint8_t* data, std::size_t size) const
{
    uint32_t crc = 0xFFFFFFFF;

    for (std::size_t i = 0; i < size; ++i) {
        const uint8_t index = static_cast<uint8_t>((crc ^ data[i]) & 0xFF);
        crc = (crc >> 8) ^ table_[index];
    }

    return crc ^ 0xFFFFFFFF;
}

uint32_t Checksum::crc32_file(const std::filesystem::path& file_path) const
{
    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for CRC32");
    }

    uint32_t crc = 0xFFFFFFFF;

    std::array<char, 8192> buffer{};

    while (file) {
        file.read(buffer.data(), buffer.size());
        const std::streamsize bytes_read = file.gcount();

        for (std::streamsize i = 0; i < bytes_read; ++i) {
            const auto byte = static_cast<uint8_t>(buffer[i]);
            const uint8_t index = static_cast<uint8_t>((crc ^ byte) & 0xFF);
            crc = (crc >> 8) ^ table_[index];
        }
    }

    return crc ^ 0xFFFFFFFF;
}