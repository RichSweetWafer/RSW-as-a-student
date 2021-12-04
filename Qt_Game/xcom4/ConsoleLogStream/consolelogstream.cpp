#include "consolelogstream.h"

std::ostream &ConsoleLogStream::get_stream()
{
    return std::cout;
}
