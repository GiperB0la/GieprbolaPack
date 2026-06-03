#pragma once

enum class ArchiveType 
{
    Zip,
    Rar
};

enum class CompressionMode
{
    Store,      // без сжатия
    Deflate     // ZIP Deflate
};

enum class ArchiveOpenMode 
{
    Read,
    Write
};