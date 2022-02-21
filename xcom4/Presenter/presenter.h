#ifndef PRESENTER_H
#define PRESENTER_H
#include <QApplication>
#include "../Logger/logger.h"
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <exception>

template<class T>
class Presenter : IPresenter
{
private:
    std::shared_ptr<Game> game;
    std::shared_ptr<T> rules;
    bool death_flag;
    bool victory_flag;
    std::shared_ptr<QApplication> app;
    std::shared_ptr<Widget> widget;
    std::shared_ptr<Logger> logger;

public:
    Presenter(int argc, char *argv[], Controls controls = Controls())
        :death_flag(false), victory_flag(false)
    {
        rules.reset(new T);
        app.reset(new QApplication(argc, argv));
        game.reset(new Game(rules->return_enemies(),
                            rules->return_items()));
        widget.reset(new Widget(controls,
                                game->field.get_width(),
                                game->field.get_height(),
                                this));
        logger = Logger::get_instance(game);
    };
    void move_up()
    {
        if (game->player.get_direction()!= UP)
            game->player.set_direction(UP);
        else if (game->player.get_position().first != game->field.get_height() - 1)
            game->movement(UP);
        update();
    }
    void move_down()
    {
        if (game->player.get_direction() != DOWN)
            game->player.set_direction(DOWN);
        else if (game->player.get_position().first != 0)
            game->movement(DOWN);
        update();
    }
    void move_left()
    {
        if (game->player.get_direction() != LEFT)
            game->player.set_direction(LEFT);
        else if (game->player.get_position().second != 0)
            game->movement(LEFT);
        update();
    }
    void move_right()
    {
        if (game->player.get_direction() != RIGHT)
            game->player.set_direction(RIGHT);
        else if (game->player.get_position().second != game->field.get_width() - 1)
            game->movement(RIGHT);
        update();
    }
    void player_attack()
    {
        game->player_attack();
        update();
    }

    void update()
    {
        game->enemy_turn();
        widget->Print(game->field, game->player);
        logger->make_log();
        if (static_cast<int>(game->player.get_hp()) <= 0)
            death_flag = true;
        if (game->player.get_position().first == game->field.get_height() - 1
                             && game->player.get_position().second == game->field.get_width() - 1)
            victory_flag = true;
        if (death_flag || victory_flag)
        {
            QMessageBox::information(widget.get(), death_flag ? "Death" : "Victory!", death_flag ? "You've been put to sleep..." : "You've reached the exit!");
            app->exit();
        }
    }

    bool save()
    {
        QString filename = (QFileDialog::getOpenFileName(
                                widget.get(),
                                "Choose file to make a save",
                                QDir::homePath(),
                                "Text (*.json)"));
        if (!filename.isEmpty())
        {
            QFile saveFile(filename);
            if (!saveFile.open(QIODevice::WriteOnly)) {
                    qWarning("Couldn't open save file.");
                    return false;
            }
            QJsonObject obj;
            write(obj);
            saveFile.write(QJsonDocument(obj).toJson());
            return true;
        }
        return false;
    }

    void write(QJsonObject &json)
    {
        QJsonObject playerObject;
        game->player.write(playerObject);
        json["player"] = playerObject;

        QJsonArray enemyArray;
        for (size_t i = 0; i < game->enemies.size(); i++)
        {
            QJsonObject enemy;
            game->enemies[i].write(enemy);
            enemyArray.append(enemy);
        }
        json["enemies"] = enemyArray;

        QJsonArray itemArray;
        for (size_t i = 0; i < game->items.size(); i++)
        {
            QJsonObject item;
            game->items[i].write(item);
            itemArray.append(item);
        }
        json["items"] = itemArray;


        QJsonArray tileArray;
        for (size_t i = 0; i < game->field.get_height(); i++)
        {
            for (size_t k = 0; k < game->field.get_width(); k++)
            {
                std::cout << i << ' ' << k << '\n';
                QJsonObject tile;
                game->field[i][k].write(tile);
                tileArray.append(tile);
            }
        }
        json["tiles"] = tileArray;
    };

    int exec()
    {
        ConsoleLogStream console_stream;
        FileLogStream file_stream;

        QMessageBox::StandardButton button = QMessageBox::question(widget.get(), "Console log", "Do you want to add logging to console?");
        if (button == QMessageBox::Yes)
            logger->track(console_stream);

        QString filename = (QFileDialog::getOpenFileName(
                                widget.get(),
                                "Choose file to make logs (optional)",
                                QDir::homePath(),
                                "Text (*.txt)"));
        if (!filename.isEmpty())
        {
            file_stream.open_file(filename.toStdString());
            logger->track(file_stream);
        }

//        QString filesave = (QFileDialog::getOpenFileName(
//                                widget.get(),
//                                "Choose file to load (optional)",
//                                QDir::homePath(),
//                                "Text (*.txt)"));
//        if (!filesave.isEmpty())
//        {
//            load(filesave);
//        }

        widget->show();
        widget->Print(game->field, game->player);
        return app->exec();
    };
};
#endif // PRESENTER_H
