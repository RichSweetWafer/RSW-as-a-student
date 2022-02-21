#include "logger.h"

std::shared_ptr<Logger> Logger::instance = nullptr;


Logger::Logger(std::shared_ptr<Game> game):game(game){}


std::shared_ptr<Logger> Logger::get_instance(std::shared_ptr<Game> game_)
{
    if (instance.get() == nullptr)
    {
        instance.reset(new Logger(game_));
    }
    return instance;
}

void Logger::track(LogStream &stream)
{
    streams.push_back(&stream);
}


void Logger::untrack(LogStream &stream)
{
    for (size_t i = 0; i < streams.size(); i++)
        if (streams[i] == &stream)
        {
            streams.erase(streams.begin() + 1);
            break;
        }
}


std::string Logger::player_log()
{
    std::string direction[] = {"UP", "DOWN", "LEFT", "RIGHT"};
    std::string player = "Player:\n  Position: ";
    QPair<size_t, size_t> pos = game->player.get_position();
    player.append(std::to_string(pos.second) +':' + std::to_string(pos.first) + '\n'
                  + "  Direction: " + direction[game->player.get_direction()] + '\n'
                  + "  HP:" + std::to_string(game->player.get_hp()) + '\n'
                  + "  MP:" + std::to_string(game->player.get_mp()) + '\n'
                  + "  strength:" + std::to_string(game->player.get_strength()) + '\n');
    return player;
}


std::string Logger::enemy_log(size_t i)
{
    std::string type[] = {"KAMIKAZE", "ARCHER", "MELEE"};
    std::string enemy = "Enemy " + std::to_string(i + 1) + ":\n  Position: ";
    QPair<size_t, size_t> pos = game->enemies[i].get_position();
    enemy.append(std::to_string(pos.second) +':' + std::to_string(pos.first) + '\n'
                  + "  Type:" + type[game->enemies[i].get_enemy_type()] + '\n'
                  + "  HP:" + std::to_string(game->enemies[i].get_hp()) + '\n'
                  + "  MP:" + std::to_string(game->enemies[i].get_mp()) + '\n'
                  + "  strength:" + std::to_string(game->enemies[i].get_strength()) + '\n');
    return enemy;
}


std::string Logger::item_log(size_t i)
{
    std::string type[] = {"HEALTH", "STRENGTH", "MANA"};
    std::string item = "Item " + std::to_string(i + 1) + ":\n  Position: ";
    QPair<size_t, size_t> pos = game->items[i].get_position();
    item.append(std::to_string(pos.second) +':' + std::to_string(pos.first) + '\n'
                  + "  Effect:" + type[game->items[i].get_effect()] + '\n'
                  + "  MP:" + std::to_string(game->items[i].get_mp()) + '\n');
    return item;
}


void Logger::make_log()
{
    QDateTime time = QDateTime::currentDateTime();

    for (size_t i = 0; i < streams.size(); i++)
    {
        streams[i]->get_stream() << time.toString().toStdString() << "\n\n"
                                 << game->player << "\n\nEnemies: \n";
        for (size_t k = 0; k < game->enemy_count; k++)
            streams[i]->get_stream() << k + 1 << ' ' << game->enemies[k];
        streams[i]->get_stream() << "\n\nItems: \n";
        for (size_t k = 0; k < game->item_count; k++)
            streams[i]->get_stream() << k + 1 << ' ' << game->items[k];
    }

}
