#include "game.h"

Game::Game(size_t enemies, size_t items)
    :field(Field()), player(Player(QPair<size_t, size_t>(0, 0))),
      enemy_count(enemies),item_count(items)
{
    place_items(item_count);
    place_enemies(enemy_count);
}


void Game::buff_player(eeffect buff, size_t mp)
{
//    std::cout << "buff: " << buff << "\n";
    if (buff == HEALTH)
        player.set_hp(player.get_hp() + mp);
    else if (buff == MANA)
        player.set_mp(player.get_mp() + mp);
    else if (buff == STRENGTH)
        player.set_strength(player.get_strength() + mp);
}


void Game::player_attack()
{
    if (player.get_mp() != 0)
    {
        edirection dir = player.get_direction();
        QPair<size_t,size_t> pos = player.get_position();
        player.set_mp(player.get_mp() - 10);
        int first = 0, second = 0;
        if (dir == UP)
            first = 1;
        else if (dir == DOWN)
            first = -1;
        else if (dir == RIGHT)
            second = 1;
        else if (dir == LEFT)
            second = -1;
        if (first != 0)
            for (int i = pos.first; i > -1 && i < static_cast<int>(field.get_height()); i+=first)
            {
    //            std::cout << i << std::endl;
                if (field[i][pos.second].get_state() == WALL)
                    break;
                if (field[i][pos.second].get_inhabitant() == ENEMY)
                {
                    for (size_t k = 0 ; k < enemy_count; k++)
                    {
                        if (i == static_cast<int>(enemies[k].get_position().first)
                                && pos.second == enemies[k].get_position().second)
                        {
                            enemies[k].set_hp(enemies[k].get_hp() - player.get_strength());
                            break;
                        }
                    }
                }
            }
        else if (second != 0)
            for (int i = pos.second; i > -1 && i < static_cast<int>(field.get_width()); i+=second)
            {
    //            std::cout << i << std::endl;
                if (field[pos.first][i].get_state() == WALL)
                    break;
                if (field[pos.first][i].get_inhabitant() == ENEMY)
                {
                    for (size_t k = 0 ; k < enemy_count; k++)
                    {
                        if (pos.first == enemies[k].get_position().first
                                && i == static_cast<int>(enemies[k].get_position().second))
                        {
                            enemies[k].set_hp(enemies[k].get_hp() - player.get_strength());
                            break;
                        }
                    }
                }
            }
    }
}


void Game::place_items(size_t it_count)
{
    items.clear();
    size_t count = 0;
    if (it_count > (field.pass_count / 3))
        it_count = 3;
    eeffect effect;
    while (count != it_count)
    {
        size_t y = QRandomGenerator::global()->bounded(1, static_cast<int>(field.get_height()));
        size_t x = QRandomGenerator::global()->bounded(1, static_cast<int>(field.get_width()));
        if (field[y][x].get_state() == PASS && field[y][x].get_inhabitant() == NOBODY
                && (QRandomGenerator::global()->generateDouble() < 0.25))
        {
            double chance = QRandomGenerator::global()->generateDouble();
            if (chance < 0.3)
                effect = STRENGTH;
            else if (chance < 0.6)
                effect = HEALTH;
            else
                effect = MANA;
            items.push_back(Item(effect, QPair<size_t, size_t>(y, x)));
            field[y][x].set_state(PASS);
            field[y][x].set_inhabitant(ITEM);
            field[y][x].set_object(&items.back());
            count++;
        }
    }
//    for (size_t y = 5; y < field.get_height(); y++)
//        for (size_t x = 5; x < field.get_width(); x++)
//        {
//            if (count == it_count)
//                break;
//            else if (field[y][x].get_state() == PASS && (QRandomGenerator::global()->generateDouble() < 0.25))
//            {
//                double chance = QRandomGenerator::global()->generateDouble();
//                if (chance < 0.3)
//                    effect = STRENGTH;
//                else if (chance < 0.6)
//                    effect = HEALTH;
//                else
//                    effect = MANA;
//                items.push_back(Item(effect, QPair<size_t, size_t>(y, x)));
//                field[y][x].set_state(PASS);
//                field[y][x].set_inhabitant(ITEM);
//                x += 3;
//                y += 3;
//                count++;
//            }
//        }
//    if (count == 0)
//        place_items(it_count);
    item_count = count;
}


void Game::place_enemies(size_t en_count)
{
    enemies.clear();
    size_t count = 0;
    if (en_count > (field.pass_count / 3))
        en_count = 3;
    eenemytype type;
    while (count != en_count)
    {
        size_t y = QRandomGenerator::global()->bounded(1, static_cast<int>(field.get_height()));
        size_t x = QRandomGenerator::global()->bounded(1, static_cast<int>(field.get_width()));
        if (field[y][x].get_state() == PASS && field[y][x].get_inhabitant() == NOBODY
                && (QRandomGenerator::global()->generateDouble() < 0.25))
        {
            double chance = QRandomGenerator::global()->generateDouble();
            if (chance < 0.3)
                type = KAMIKAZE;
            else if (chance < 0.6)
                type = ARCHER;
            else
                type = MELEE;
            enemies.push_back(Enemy(type, QPair<size_t, size_t>(y, x)));
            field[y][x].set_state(PASS);
            field[y][x].set_inhabitant(ENEMY);
            field[y][x].set_object(&enemies.back());
            count++;
        }
    }
//    for (size_t y = 5; y < field.get_height(); y++)
//        for (size_t x = 5; x < field.get_width(); x++)
//        {
//            if (count == en_count)
//                break;
//            else if (field[y][x].get_state() == PASS && (QRandomGenerator::global()->generateDouble() < 0.25))
//            {
//                double chance = QRandomGenerator::global()->generateDouble();
//                if (chance < 0.3)
//                    type = KAMIKAZE;
//                else if (chance < 0.6)
//                    type = ARCHER;
//                else
//                    type = MELEE;
//                enemies.push_back(Enemy(type, QPair<size_t, size_t>(y, x)));
//                field[y][x].set_state(PASS);
//                field[y][x].set_inhabitant(ENEMY);
//                x += 3;
//                y += 3;
//                count++;
//            }
//        }
//    if (count == 0)
//        place_items(en_count);
    enemy_count = count;
}


void Game::enemy_turn()
{
    QPair<size_t,size_t> player_pos = player.get_position();
    for (size_t i = 0; i < enemy_count; i++)
    {
        eenemytype type = enemies[i].get_enemy_type();
        QPair<size_t, size_t> en_pos = enemies[i].get_position();
        QPair<int, int> dist(static_cast<int>(en_pos.first - player_pos.first),
                             static_cast<int>(en_pos.second - player_pos.second));
        if (enemies[i].get_hp() <= 0)
        {
            // ENEMY DEATH
            enemies.erase(std::vector<Enemy>::iterator(enemies.begin() + i));
            field[en_pos.first][en_pos.second].set_inhabitant(NOBODY);
            field[en_pos.first][en_pos.second].set_object(nullptr);
            enemy_count--;
            i--;
            continue;
        }
        if (type == KAMIKAZE || type == MELEE)
        {
            if ((std::abs(dist.first) < 1 && std::abs(dist.second) == 1)
                    || (std::abs(dist.first) == 1 && std::abs(dist.second) < 1))
            {
                enemy_attack(enemies[i].get_strength());
                if (type == KAMIKAZE)
                {
                    // KAMIKAZE BOOM & DEATH
                    enemies[i].set_hp(0);
                    enemies.erase(std::vector<Enemy>::iterator(enemies.begin() + i));
                    field[en_pos.first][en_pos.second].set_inhabitant(NOBODY);
                    field[en_pos.first][en_pos.second].set_object(nullptr);
                    enemy_count--;
                    i--;
                }
            }
            else if (std::pow(std::abs(dist.first), 2) + std::pow(std::abs(dist.second), 2)
                     <= 25)
            {
                QPair<size_t, size_t> en_pos = enemies[i].get_position();
                int first = 0, second = 0;
                if (dist.first > 0)
                    first = -1;
                else if (dist.first < 0)
                    first = 1;
                if (dist.second > 0)
                    second = -1;
                else if (dist.second < 0)
                    second = 1;
                if (field[en_pos.first+first][en_pos.second+second].get_inhabitant() == NOBODY
                        && field[en_pos.first+first][en_pos.second+second].get_state() == PASS)
                {
                    field[en_pos.first][en_pos.second].set_inhabitant(NOBODY);
                    field[en_pos.first][en_pos.second].set_object(nullptr);
                    enemies[i].set_position(en_pos.first+first, en_pos.second+second);
                    field[en_pos.first+first][en_pos.second+second].set_inhabitant(ENEMY);
                    field[en_pos.first+first][en_pos.second+second].set_object(&enemies[i]);
                }
            }
        }
        else
        {
            if (std::abs(dist.first) <= 10 || std::abs(dist.second) <= 10)
            {
                int first = 0, second = 0;
                if (dist.first != 0)
                {
                    first = dist.first < 0 ? 1 : -1;
                    for (int k = en_pos.first; k > -1 && k < static_cast<int>(field.get_height()); k+=first)
                    {
                        if (field[k][en_pos.second].get_state() == WALL)
                            break;
                        if (field[k][en_pos.second].get_inhabitant() == PLAYER)
                        {
                            if (k == static_cast<int>(player.get_position().first)
                                    && en_pos.second == player.get_position().second)
                                enemy_attack(enemies[i].get_strength());
                        }
                    }
                }
                else if (dist.second != 0)
                {
                    second = dist.second < 0 ? 1 : -1;
                    for (int k = en_pos.second; k > -1 && k < static_cast<int>(field.get_width()); k+=second)
                    {
                        if (field[en_pos.first][k].get_state() == WALL)
                            break;
                        if (field[en_pos.first][k].get_inhabitant() == PLAYER)
                        {
                            if (k == static_cast<int>(player.get_position().second)
                                    && en_pos.first == player.get_position().first)
                                enemy_attack(enemies[i].get_strength());
                        }
                    }
                }
            }
        }
    }
}


void Game::enemy_attack(size_t damage)
{
    player.set_hp(player.get_hp() - damage);
}


void Game::movement(edirection dir)
{
    QPair<size_t, size_t> position = player.get_position();
    short int first = 0, second = 0;
    if (dir == UP)
        first = 1;
    else if (dir == DOWN)
        first = -1;
    else if (dir == RIGHT)
        second = 1;
    else if (dir == LEFT)
        second = -1;
    if (field[position.first + first][position.second + second].get_state() != WALL)
    {
        if (field[position.first + first][position.second + second].get_inhabitant() == NOBODY)
        {
            field[position.first][position.second].set_inhabitant(NOBODY);
            field[position.first][position.second].set_object(nullptr);
            field[position.first + first][position.second + second].set_inhabitant(PLAYER);
            field[position.first + first][position.second + second].set_object(&player);
            player.set_position(position.first + first, position.second + second);
        }
        else if (field[position.first + first][position.second + second].get_inhabitant() == ITEM)
        {
            QPair<size_t, size_t> item_pos(position.first + first, position.second + second);
            for (size_t i = 0; i < item_count; i++)
            {
                if (items[i].get_position() == item_pos)
                {
                    buff_player(items[i].get_effect(), items[i].get_mp());
                    field[position.first][position.second].set_inhabitant(NOBODY);
                    field[position.first][position.second].set_object(nullptr);
                    field[position.first + first][position.second + second].set_inhabitant(PLAYER);
                    field[position.first + first][position.second + second].set_object(&player);

                    player.set_position(position.first + first, position.second + second);
                    items.erase(std::vector<Item>::iterator(items.begin() + i));
                    item_count--;
                    break;
                }
            }
        }
    }
}


Game& Game::operator=(const Game& game)
{
    if (&game != this)
    {
        field = game.field;
        player = game.player;
        enemies = game.enemies;
        enemy_count = game.enemy_count;
        items = game.items;
        item_count = game.item_count;
    }
    std::cout << "Assignment operator worked here!\n";
    return *this;
}
