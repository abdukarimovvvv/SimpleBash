#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#define BUFFER_SIZE 5000

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
  int e; /*Шаблон*/
  int i; /*Игнорирует различия регистра.*/
  int v; /*Инвертирует смысл поиска соответствий.*/
  int c; /*Выводит только количество совпадающих строк.*/
  int l; /*Выводит только совпадающие файлы.*/
  int n; /*Предваряет каждую строку вывода номером строки из файла ввода.*/
  int h; /*Выводит совпадающие строки, не предваряя их именами файлов.*/
  int s; /*Подавляет сообщения об ошибках о несуществующих или нечитаемых
            файлах.*/
  int f; /*Получает регулярные выражения из файла.*/
  int o; /*Печатает только совпадающие (непустые) части совпавшей строки.*/
};

void flag_parser(int argc, char **argv, struct flags *flag, char *patternE);
void processing(struct flags *flag, FILE *fp, regex_t reg, char *file);
void grep_file(struct flags *flag, char *pattern, char *file);
int f_flag(char *pattern, char *temp);
void grep(struct flags *flag, int argc, char **argv, char *temp);
void initStruct(struct flags *flag);

#endif  // SRC_GREP_S21_GREP_H_