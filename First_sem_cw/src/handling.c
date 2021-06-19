#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include "structs.h"
#include "handling.h"

void print(Text * text){
  for (int t = 0; t < text->sentences_count; t++){
    for (int s = 0; s < text->sentence_p[t].words_count; s++){
      text->sentence_p[t].word_p[s].appearence = 1;
    }
  }
  for (int t = 0; t < text->sentences_count; t++){
    for (int s = 0; s < text->sentence_p[t].words_count; s++){
      if (text->sentence_p[t].word_p[s].appearence == 1) {
        for (int i = s+1; i < text->sentence_p[t].words_count; i++) {
          if (wcscasecmp(text->sentence_p[t].word_p[s].entity, text->sentence_p[t].word_p[i].entity) == 0) {
            text->sentence_p[t].word_p[s].appearence += 1;
            text->sentence_p[t].word_p[i].appearence += 1;
          }
          for (int t1 = t+1; t1 < text->sentences_count; t1++) {
            for (int s1 = 0; s1 < text->sentence_p[t1].words_count; s1++) {
              if (wcscasecmp(text->sentence_p[t].word_p[s].entity, text->sentence_p[t1].word_p[s1].entity) == 0) {
                text->sentence_p[t].word_p[s].appearence += 1;
                text->sentence_p[t1].word_p[s1].appearence += 1;
              }
            }
          }
        }
      }
    }
  }
  for (int t = 0; t < text->sentences_count; t++){
    for (int s = 0; s < text->sentence_p[t].words_count; s++){
      if (text->sentence_p[t].word_p[s].appearence == 1){
        wprintf(text->sentence_p[t].word_p[s].entity);
        wprintf(L"\n");
      }
    }
  }
}


void date(Text * text){
  int year;
  wchar_t month[3];
  wchar_t days[31][3] = {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18", L"19", L"20",
                           L"21", L"22", L"23", L"24", L"25", L"26", L"27", L"28", L"29", L"30", L"31",};
  wchar_t months[12][9] = {L"января", L"февраля", L"марта", L"апреля", L"мая", L"июня", L"июля", L"августа", L"сентября", L"октября", L"ноября", L"декабря"};
  for (int t = 0; t < text->sentences_count; t++){
    for (int s = 0; s < text->sentence_p[t].words_count-3; s++){
      if(wcscasecmp(text->sentence_p[t].word_p[s+3].entity, L"г") == 0 && text->sentence_p[t].word_p[s+2].symbols_count == 5){
        for (int i = 0; i < 31; i++) {
            if (wcscmp(text->sentence_p[t].word_p[s].entity, days[i]) == 0){
              for (int j = 0; j < 12; j++) {
                if(wcscasecmp(text->sentence_p[t].word_p[s+1].entity, months[j]) == 0){
                  swprintf(month, 3, L"%d", j+1);
                  wchar_t *p = &text->sentence_p[t].word_p[s+2].entity[text->sentence_p[t].word_p[s+2].symbols_count];
                  year = wcstol(text->sentence_p[t].word_p[s+2].entity, &p, 10);
                  if (year > 999){
                    text->sentence_p[t].word_p[s].entity = realloc(text->sentence_p[t].word_p[s].entity, 30 * sizeof(wchar_t));
                    wcscat(text->sentence_p[t].word_p[s].entity, L"/");
                    wcscat(text->sentence_p[t].word_p[s].entity, month);
                    wcscat(text->sentence_p[t].word_p[s].entity, L"/");
                    wcscat(text->sentence_p[t].word_p[s].entity, text->sentence_p[t].word_p[s+2].entity);
                    text->sentence_p[t].word_p[s].sign = 0;
                    text->sentence_p[t].word_p[s].symbols_count += text->sentence_p[t].word_p[s+1].symbols_count + text->sentence_p[t].word_p[s+2].symbols_count + text->sentence_p[t].word_p[s+3].symbols_count - 3;
                    text->sentence_p[t].words_count -= 3;
                    for (size_t z = 1; z < 4; z++) {
                      free(text->sentence_p[t].word_p[s+z].entity);
                      text->sentence_p[t].word_p[s+z].sign = 0;
                      text->sentence_p[t].word_p[s+z].symbols_count = 0;
                    }
                    break;
                  }
                }
              }
            }
          }
        }
      }
    }
  }


  void sort(Text * text){
    Sentence p;
    for (int t = 0; t < text->sentences_count; t++) {
      text->sentence_p[t].words_length_multiplication = 1;
      for (int s = 0; s < text->sentence_p[t].words_count; s++) {
        text->sentence_p[t].words_length_multiplication *= text->sentence_p[t].word_p[s].symbols_count;
      }
    }
    for(int i = 0; i < text->sentences_count; i++){
      for (int j = i+1; j < text->sentences_count; j++) {
        if (text->sentence_p[i].words_length_multiplication > text->sentence_p[j].words_length_multiplication){
          p = text->sentence_p[i];
          text->sentence_p[i] = text->sentence_p[j];
          text->sentence_p[j] = p;
        }
      }
    }
  }


  void delete(Text * text){
    for (int t = 0; t < text->sentences_count; t++) {
      text->sentence_p[t].symbol = 0;
      text->sentence_p[t].number = 0;
    }
    for (int t = 0; t < text->sentences_count; t++){
      for (int s = 0; s < text->sentence_p[t].words_count; s++){
        if (wcsstr(text->sentence_p[t].word_p[s].entity, L"#") != NULL)
          text->sentence_p[t].symbol = 1;
        else if (wcsstr(text->sentence_p[t].word_p[s].entity, L"№") != NULL)
          text->sentence_p[t].symbol = 1;
        for (int w = 0; w < text->sentence_p[t].word_p[s].symbols_count; w++)
          if (iswdigit(text->sentence_p[t].word_p[s].entity[w]))
            text->sentence_p[t].number = 1;
      }
      if (text->sentence_p[t].number != 1 && text->sentence_p[t].symbol == 1){
        for (int s = 0; s < text->sentence_p[t].words_count; s++){
          free(text->sentence_p[t].word_p[s].entity);
        }
        free(text->sentence_p[t].word_p);
        text->sentence_p[t].words_count = 0;
      }
    }
  }


  void annotation(){
    wprintf(L"\nНапишите \"1\", чтобы вывести текст.\n");
    wprintf(L"Напишите \"2\", чтобы распечатать все слова, встречающиеся в тексте не более одного раза.\n");
    wprintf(L"Напишите \"3\", чтобы заменить строку вида \"<день> <месяц> <год> г.\" на \"ДД/ММ/ГГГГ\".\n");
    wprintf(L"Напишите \"4\", чтобы отсортировать предложения в порядке неубывания по произведению длин содержащихся в них слов.\n");
    wprintf(L"Напишите \"5\", чтобы удалить все предложения, содержащие \'#\' или \'№\', но не числа.\n");
    wprintf(L"Напишите \"0\", чтобы прекратить выполнение программы.\n");
  }
