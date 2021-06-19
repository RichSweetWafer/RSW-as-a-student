#pragma once
#include <wchar.h>

typedef struct Word{
  wchar_t *entity;
  int appearence;
  int symbols_count;
  int sign;
} Word;
typedef struct Sentence{
  Word *word_p;
  int words_count;
  long int words_length_multiplication;
  int symbol;
  int number;
} Sentence;
typedef struct Text{
  Sentence *sentence_p;
  int sentences_count;
} Text;
