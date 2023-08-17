#include "s21_grep.h"

void flag_parser(
    int argc, char **argv, struct flags *flag,
    char *patternE) {  // argc — это счетчик числа аргументов командной строки
  // argv — указатель на массив одно - или многобайтовых строк
  int opt = 0;
  const char *s_options = "e:ivclnhsf:o";
  while ((opt = getopt(argc, argv, s_options)) != -1) {
    switch (opt) { /*обработка флагов*/
      case 'e':
        flag->e = 1;
        snprintf(patternE, BUFFER_SIZE, "%s",
                 optarg);  // snprintf() - Записать вывод в символьный массив
                           // дозаданного количества символов
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        snprintf(patternE, BUFFER_SIZE, "%s", optarg);
        break;
      case 'o':
        flag->o = 1;
        break;
      default:
        printf("error\n");
        break;
    }
  }
}

void processing(struct flags *flag, FILE *fp, regex_t reg,
                char *file) {  // обработка файла
  char text[BUFFER_SIZE] = {0};
  regmatch_t pmatch[1];  // подготавливаем слово к поиску
  // regmatch_t - Структура с результатами сравнения строки с регулярным
  // выражением. pmatch - Массив объектов regmatch_t, в котором функция может
  // хранить совпадения.
  int line_matches = 0, nline = 1;
  while (fgets(text, BUFFER_SIZE - 1, fp) != NULL) {
    int match = 0;
    int success = regexec(&reg, text, 1, pmatch, 0);  // ищем слово
    // regexec - Сравнить строку с составленным регулярным выражением
    if (strchr(text, '\n') == NULL) strcat(text, "\n");
    if (success == 0 && !flag->v) match = 1;
    if (success == REG_NOMATCH && flag->v) match = 1;
    if (match && !flag->l && !flag->c && flag->n) printf("%d:", nline);
    if (match && !flag->l && !flag->c && !flag->o) printf("%s", text);
    if (match && flag->o) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo;
           i++) {  // rm_so - Смещение байта от начала строки до начала
                   // подстроки.
        // rm_eo - Смещение байта от начала строки первого символа после
        // окончания подстроки.
        printf("%c", text[i]);
      }
      printf("\n");
    }
    line_matches += match;
    nline++;
  }
  if (flag->l && line_matches > 0) printf("%s\n", file);
  if (flag->c && !flag->l) printf("%d\n", line_matches);
}

void grep_file(struct flags *flag, char *pattern, char *filename) {
  int cflag = REG_EXTENDED;
  regex_t reg;
  FILE *file;
  file = fopen(filename, "r");
  if (flag->i) cflag = REG_ICASE;
  if (file != NULL) {
    regcomp(&reg, pattern, cflag);  // regcomp - Составить регулярное выражение
    processing(flag, file, reg, filename);
    regfree(&reg);  // regfree - Освободить память, выделенную под регулярное
                    // выражение
    fclose(file);
  }
}

int f_flag(char *pattern, char *filename) {  // обработка f флага
  FILE *file;
  file = fopen(filename, "r");
  int i = 0;
  if (file != NULL) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
      if (ch == 13 || ch == 10) pattern[i++] = '|';
      if (ch != 13 && ch != 10) pattern[i++] = ch;
    }
  } else {
    printf("error\n");
    i = -1;
  }
  if (pattern[i - 1] == '|') pattern[i - 1] = '\0';
  fclose(file);
  return i;
}

void grep(struct flags *flag, int argc, char **argv, char *buff) {
  char pattern[BUFFER_SIZE] = {0};
  int f_patt = 0;
  if (!flag->f && !flag->e)
    snprintf(pattern, BUFFER_SIZE, "%s", argv[optind++]);
  if (flag->f) f_patt = f_flag(pattern, buff);
  if (!flag->f && flag->e) snprintf(pattern, BUFFER_SIZE, "%s", buff);
  if (f_patt != -1) {
    int file_c = 0;
    if (argc - optind > 1) file_c = 1;
    for (int i = optind; i < argc; i++) {
      if (file_c && !flag->h && !flag->l) printf("%s:", argv[i]);
      grep_file(flag, pattern, argv[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  struct flags flag = {0};
  char patternE[BUFFER_SIZE] = {0};
  flag_parser(argc, argv, &flag, patternE);
  if (argc >= 3) grep(&flag, argc, argv, patternE);
  return 0;
}