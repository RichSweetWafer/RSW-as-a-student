#include "Presenter/presenter.h"

int main(int argc, char *argv[])
{
    Presenter<Regulator<3, 5>> presenter(argc, argv);
    return presenter.exec();
}

// Lab 1 *DONE*
// Реализовать возможность работы с объектом через указатель в объекте клетки *DONE*

// Lab 2 *DONE*
// Избежать появления 0 врагов и 0 предметов. *DONE*

// Lab 3 *DONE*
// Сделать перегрузку << *DONE*

// Lab 4 *DONE*
// Исправить 2 и 3 лабы *DONE*

// Lab 5 *DONE*
//


/* Patterns:
 * 1. MVP (Game, Presenter, Printer)
 * 2. Singleton (Logger)
 * 3. Observer (Logger and thread-subscribers)
 * 4.
 * 5.
*/
