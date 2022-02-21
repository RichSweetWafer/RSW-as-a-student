#ifndef LOGGER_H
#define LOGGER_H
#include "../FileLogStream/filelogstream.h"
#include <QDateTime>
#include <QString>
// Singleton
class Logger
{
private:
    Logger(std::shared_ptr<Game> game);
    static std::shared_ptr<Logger> instance;
    std::shared_ptr<Game> game;
    std::vector<LogStream *> streams;
public:
    Logger(Logger &other) = delete;
    void operator=(const Logger &) = delete;
    static std::shared_ptr<Logger> get_instance(std::shared_ptr<Game> game_);
    void track(LogStream &stream);
    void untrack(LogStream &stream);
    std::string player_log();
    std::string enemy_log(size_t i);
    std::string item_log(size_t i);
    void make_log();
};

#endif // LOGGER_H
