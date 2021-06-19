#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include "structs.h"
#include "input.h"
#include "handling.h"

int main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");
    int par;
    Text text;

    wprintf(L"Введите текст:\n");

    input(&text);
    filter(&text);

    wprintf(L"Конец ввода.\nВведенный текст:\n");

    output(&text);

    wprintf(L"\nВыберите действие над текстом: ");
    annotation();

    while (1){
      wscanf(L"%d", &par);

      switch (par) {
        case 1:
          output(&text);
          break;

        case 2:
          print(&text);
          break;

        case 3:
          date(&text);
          break;

        case 4:
          sort(&text);
          break;

        case 5:Ы
          delete(&text);
          break;

        case 0:
          wprintf(L"Exiting the program...\n");
          //ОСВОБОЖДЕНИЕ ПАМЯТИ
          for (int t = 0; t < text.sentences_count; t++){
            for (int s = 0; s < text.sentence_p[t].words_count; s++){
              free(text.sentence_p[t].word_p[s].entity);
            }
            if (text.sentence_p[t].words_count == 0)
              continue;
            free(text.sentence_p[t].word_p);
          }
          free(text.sentence_p);
          // ОСВОБОЖДЕНИЕ ПЯМЯТИ
          return 0;

        default:
          wprintf(L"Введено неверное значение.");
          annotation();
          break;
      }
    }
}
