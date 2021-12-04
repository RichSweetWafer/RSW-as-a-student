#ifndef CONSOLELOGSTREAM_H
#define CONSOLELOGSTREAM_H
#include "../LogStream/logstream.h"

class ConsoleLogStream : public LogStream
{
public:
    std::ostream &get_stream();
};

#endif // CONSOLELOGSTREAM_H
