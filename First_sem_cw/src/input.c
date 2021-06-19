#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include "structs.h"
#include "input.h"

void input(Text * text){
  wchar_t ch;
  wchar_t prev_ch;
  text->sentences_count = 1;

  text->sentence_p = calloc(1, sizeof(Sentence));
  text->sentence_p->words_count = 1;

  text->sentence_p->word_p = calloc(1, sizeof(Word));
  text->sentence_p->word_p[0].symbols_count = 1;
  text->sentence_p->word_p[0].entity = calloc(2, sizeof(wchar_t));

  while(ch = getwchar()){
    if (prev_ch == L'.'){
      if (ch != L'\n'){
        text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].sign = 0;
        text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity[text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].symbols_count-1] = L'\0';
        text->sentences_count += 1;
        text->sentence_p = realloc(text->sentence_p, text->sentences_count * sizeof(Sentence));
        text->sentence_p[text->sentences_count - 1].words_count = 1;
        text->sentence_p[text->sentences_count - 1].word_p = calloc(1, sizeof(Word));
        text->sentence_p[text->sentences_count - 1].word_p[0].symbols_count = 1;
        text->sentence_p[text->sentences_count - 1].word_p[0].entity = calloc(2, sizeof(wchar_t));
      }
      else break;
    }
    else if (ch == L' ' || ch == L','){
      if (ch == L' '){
        text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].sign = 1;
      }
      else if (ch == L','){
        text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].sign = 2;
      }
      text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity[text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].symbols_count-1] = L'\0';
      text->sentence_p[text->sentences_count - 1].words_count += 1;
      text->sentence_p[text->sentences_count - 1].word_p = realloc(text->sentence_p[text->sentences_count - 1].word_p, text->sentence_p[text->sentences_count - 1].words_count * sizeof(Word));
      text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].symbols_count = 1;
      text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity = calloc(2, sizeof(wchar_t));
      continue;
    }
    else if (ch == L'.'){
      prev_ch = L'.';
      continue;
    }
    text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity[text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].symbols_count - 1] = ch;
    text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity = realloc(text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity, (++text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].symbols_count + 1)*sizeof(wchar_t));
    prev_ch = ch;
  };
  text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].entity[text->sentence_p[text->sentences_count - 1].word_p[text->sentence_p[text->sentences_count - 1].words_count - 1].symbols_count-1] = L'\0';
}

void filter(Text * text){
  int copy = 0;
  for (int t = 0; t < text->sentences_count; t++){
    for (int s = t+1; s < text->sentences_count; s++){
      if (text->sentence_p[t].words_count == text->sentence_p[s].words_count){
        copy = 1;
        for (int i = 0; i < text->sentence_p[t].words_count; i++){
          if (wcscasecmp(text->sentence_p[t].word_p[i].entity, text->sentence_p[s].word_p[i].entity) != 0){
            copy = 0;
            break;
          }
        }
        if (copy == 1){
          for (int w = 0; w < text->sentence_p[s].words_count; w++){
            free(text->sentence_p[s].word_p[w].entity);
          }
          free(text->sentence_p[s].word_p);
          text->sentence_p[s].words_count = 0;
        }
      }
    }
  }
}

void output(Text * text){
  for (int t = 0; t < text->sentences_count; t++){
    for (int s = 0; s < text->sentence_p[t].words_count; s++){
      wprintf(text->sentence_p[t].word_p[s].entity);
      if (s !=  text->sentence_p[t].words_count - 1)
        if (text->sentence_p[t].word_p[s].sign == 1) wprintf(L" ");
        else if (text->sentence_p[t].word_p[s].sign == 2) wprintf(L",");
    }
    if (text->sentence_p[t].words_count == 0)
      continue;
    wprintf(L".");
  }
  wprintf(L"\n");
}
