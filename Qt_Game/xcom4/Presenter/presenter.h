#ifndef PRESENTER_H
#define PRESENTER_H
#include <QApplication>
#include "../Logger/logger.h"
#include <QFileDialog>
#include <QMessageBox>

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

        widget->show();
        widget->Print(game->field, game->player);
        return app->exec();
    };
};

//template<class T>
//Presenter<T>::Presenter(Printer * view):view(view),exit_flag(false)
//{
//    rules.reset(new T);
//    game.reset(new Game(rules->return_enemies(), rules->return_items()));
//}
//template<class T>
//std::string Presenter<T>::get_log_file_name()
//{
//    std::string file;
//    view->Print("Please, write down the name of the log file:");
//    std::cin >> file;
//    return file;
//}
//template<class T>
//void Presenter<T>::key_press_event(/*QKeyEvent * event*/)
//{
//    // UP
//    char ch;
//    std::cin >> ch;
//    QPair<size_t, size_t> position = game->player.get_position();
//    //if (event->key() == Qt::Key_W)
//    if (toupper(ch) == 'W')
//    {
//        if (game->player.get_direction()!= UP)
//            game->player.set_direction(UP);
//        else if (position.first != game->field.get_height() - 1)
//            game->movement(UP);
//    }
//    // DOWN
//    //else if (event->key() == Qt::Key_S)
//    else if (toupper(ch) == 'S')
//    {
//        if (game->player.get_direction() != DOWN)
//            game->player.set_direction(DOWN);
//        else if (position.first != 0)
//            game->movement(DOWN);
//    }
//    // RIGHT
//    else if (toupper(ch) == 'D')
//    //else if (event->key() == Qt::Key_D)
//    {
//        if (game->player.get_direction() != RIGHT)
//            game->player.set_direction(RIGHT);
//        else if (position.second != game->field.get_width() - 1)
//            game->movement(RIGHT);
//    }
//    // LEFT
//    //else if (event->key() == Qt::Key_A)
//    else if (toupper(ch) == 'A')
//    {
//        if (game->player.get_direction() != LEFT)
//            game->player.set_direction(LEFT);
//        else if (position.second != 0)
//            game->movement(LEFT);
//    }
//    //else if (event->key() == Qt::Key_Space)
//    else if (toupper(ch) == 'F')
//    {
//        game->player_attack();
//    }
//    //else if (event->key() == Qt::Key_Escape)
//    else if (toupper(ch) == 'Q')
//    {
//        exit_flag = true;
//    }
//}
//template<class T>
//void Presenter<T>::execute()
//{
//    std::shared_ptr<Logger> logger = Logger::get_instance(game);
//    ConsoleLogStream console_stream;
//    FileLogStream file_stream;
//    view->Print("Welcome!");
//    do
//    {
//        char ch;
//        view->Print("Would you like to make a log file or/and print log to console?\nf(file)/c(console)/b(both)/n(no):");
//        std::cin >> ch;
//        if (toupper(ch) == 'F')
//        {
//            file_stream.open_file(get_log_file_name());
//            logger->track(file_stream);
//            break;
//        }
//        else if (toupper(ch) == 'C')
//        {
//            logger->track(console_stream);
//            break;
//        }
//        else if (toupper(ch) == 'B')
//        {
//            file_stream.open_file(get_log_file_name());
//            logger->track(file_stream);
//            logger->track(console_stream);
//            break;
//        }
//        else if (toupper(ch) == 'N')
//            break;
//        else
//            view->Print("Wrong character, try again.");
//    } while (1);
//    view->Print("Thanks! And now to play the game...");
//    QThread::sleep(2);
//    view->Print(game->field, game->player);
//    logger->make_log();
//    while (exit_flag == false)
//    {
//        key_press_event();
//        view->Print(game->field, game->player);
//        game->enemy_turn();
//        if (static_cast<int>(game->player.get_hp()) <= 0 ||
//                (game->player.get_position().first == game->field.get_height() - 1
//                 && game->player.get_position().second == game->field.get_width() - 1))
//            exit_flag = true;
//        QThread::usleep(70000);
//        view->Print(game->field, game->player);
//        logger->make_log();
//    }
//}

#endif // PRESENTER_H
