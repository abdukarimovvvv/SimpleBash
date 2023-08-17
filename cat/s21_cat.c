#include "s21_cat.h"

void parser(
    int argc, char *argv[],
    flags *flag)  //(argc — это счетчик числа аргументов командной строки)
                  //(argv — указатель на массив одно - или многобайтовых строк)
{
  int f;
  const char *f_options =
      "bEnsTvet";  // const char *. Это указывает на то, что функция будет
                   // обрабатывать символы как константы
  static struct option options[] = {
      //обрабатываем длинные флаги
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
      {0, 0, 0, 0},
  };
  while ((f = getopt_long(argc, argv, f_options, options, NULL)) !=
         -1) {    // парсинг
    switch (f) {  //обработка флагов
      case 'b':
        flag->b = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      default:
        printf("No such file or directory\n");
        break;
    }
    if (flag->b && flag->n) flag->n = 0;
  }
  return;
}

void reader(char *argv[], flags *flag) {  // чтение файла и обработка флагов
  FILE *f;
  f = fopen(argv[optind], "r");
  if (f != NULL) {
    int str_count = 1;
    int empty_str_count = 0;
    int conter = '\n';
    while (!feof(f)) {
      int c = fgetc(f);
      if (c == EOF) break;
      if (flag->s && c == '\n' && conter == '\n') {
        empty_str_count++;
        if (empty_str_count > 1) {
          continue;
        }
      } else {
        empty_str_count = 0;
      }
      if (conter == '\n' && ((flag->b && c != '\n') || flag->n))
        printf("%6d\t", str_count++);
      if (flag->t && c == '\t') {
        printf("^");
        c = 'I';
      }
      if (flag->e && c == '\n') printf("$");
      if (flag->v) {
        if ((c >= 0 && c < 9) || (c > 10 && c < 32) || (c > 126 && c <= 160)) {
          printf("^");
          if (c > 126) {
            c -= 64;
          } else {
            c += 64;
          }
        }
      }
      printf("%c", c);
      conter = c;
    }
    fclose(f);
  } else {
    // printf("No such file or directory");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  flags flag = {S21_FLAGS};  //здесь храним флаги
  parser(argc, argv, &flag);
  while (optind < argc) {
    reader(argv, &flag);
    optind++;
  }
  return 0;
}