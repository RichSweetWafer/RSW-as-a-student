#include "filelogstream.h"

std::ofstream &FileLogStream::get_stream()
{
    return file;
}

void FileLogStream::open_file(const std::string &name)
{
    if (file.is_open())
        file.close();

    file.open(name);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");
}

FileLogStream::~FileLogStream()
{
    if (file.is_open())
        file.close();
}
