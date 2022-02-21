#ifndef FILELOGSTREAM_H
#define FILELOGSTREAM_H
#include "../ConsoleLogStream/consolelogstream.h"
#include <fstream>

class FileLogStream : public LogStream
{
private:
    std::ofstream file;
public:
    std::ofstream &get_stream();
    void open_file(const std::string &name);
    ~FileLogStream();
};

#endif // FILELOGSTREAM_H
