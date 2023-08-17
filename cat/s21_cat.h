#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#define S21_FLAGS .b = 0, .e = 0, .n = 0, .s = 0, .t = 0, .v = 0,
//#define определяет идентификатор и последовательность символов

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flag {
  int b;  // нумерует непустые строки
  int e;  // выводить символ $ в конце каждой строки
  int n;  // нумерует все строки
  int s;  // подавляет повторяющиеся пустые строки
  int t;  // выводит символы TAB как ^I
  int v;
} flags;  // здесь flags - это имя нового типа

void parser(int argc, char *argv[], flags *flag);
void reader(char *argv[], flags *flag);

#endif  // SRC_CAT_S21_CAT_H_