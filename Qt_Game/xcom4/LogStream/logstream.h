#ifndef LOGSTREAM_H
#define LOGSTREAM_H
#include "../Game/game.h"

class LogStream
{
public:
    virtual std::ostream &get_stream() = 0;
};

#endif // LOGSTREAM_H
