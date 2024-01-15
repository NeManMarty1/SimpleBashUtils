
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4960

struct flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
};

void flag_parser(int argc, char **argv, struct flags *flags, char *patternE);
void processing(struct flags *flags, FILE *fp, regex_t reg, char *file);
void grep_file(struct flags *flags, char *pattern, char *file);
int f_flag(char *pattern, char *temp);
void grep(struct flags *flags, int argc, char **argv, char *temp);

int main(int argc, char *argv[]) {
  struct flags flags = {0};
  char patternE[BUFFER_SIZE] = {0};
  flag_parser(argc, argv, &flags, patternE);
  if (argc >= 3) grep(&flags, argc, argv, patternE);
  return 0;
}

void flag_parser(int argc, char **argv, struct flags *flags, char *patternE) {
  int opt = 0;
  const char *s_options = "e:ivclnhsf:o";
  while ((opt = getopt(argc, argv, s_options)) != -1) {
    switch (opt) {
      case 'e':
        flags->e = 1;
        snprintf(patternE, BUFFER_SIZE, "%s", optarg);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        snprintf(patternE, BUFFER_SIZE, "%s", optarg);
        break;
      case 'o':
        flags->o = 1;
        break;
      default:
        printf("error\n");
        break;
    }
  }
}

void processing(struct flags *flags, FILE *fp, regex_t reg,
                char *file) {  // обработка файла
  char text[BUFFER_SIZE] = {0};
  regmatch_t pmatch[1];  // подготавливаем слово к поиску
  int line_matches = 0,
      nline =
          1;  //- line_matches - счетчик количества строк, содержащих совпадения
  //- nline - счетчик номера строки
  while (fgets(text, BUFFER_SIZE - 1, fp) !=
         NULL) {  //Цикл считывающий строки из файла по одной строке за итерацию
                  //до тех пор, пока не будет достигнут конец файла.
    int match = 0;
    int success = regexec(&reg, text, 1, pmatch, 0);  // ищем слово
    if (strchr(text, '\n') == NULL) strcat(text, "\n");
    if (success == 0 && !flags->v) match = 1;
    if (success == REG_NOMATCH && flags->v) match = 1;
    if (match && !flags->l && !flags->c && flags->n) printf("%d:", nline);
    if (match && !flags->l && !flags->c && !flags->o) printf("%s", text);
    if (match && flags->o) {
      for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
        printf("%c", text[i]);
      }
      printf("\n");
    }
    line_matches += match;
    nline++;
  }
  if (flags->l && line_matches > 0) printf("%s\n", file);
  if (flags->c && !flags->l) printf("%d\n", line_matches);
}

void grep_file(struct flags *flags, char *pattern, char *filename) {
  int cflags = REG_EXTENDED;  //расширенное регулярное выражение
  regex_t reg;  //это переменная типа regex_t, которая используется для хранения
                //скомпилированного регулярного выражения
  FILE *file;                   //файл
  file = fopen(filename, "r");  //открываем файл
  if (flags->i) cflags = REG_ICASE;  //если флаг i то игнорируем регистр
  if (file != NULL) {  //если файл открылся
    regcomp(&reg, pattern,
            cflags);  // regcomp используется для обработки регулярного
                      // выражения и придания ей формы,
                      //которая пригодна для последующих regexec-поисков.
                      // regcomp поддерживается: preg, указателем на область
                      // хранения буферного шаблона
    processing(flags, file, reg,
               filename);  //передаем в функцию обработки файла структуру с
                           //флагами файл регулярные выражения и название файла
                           //для дальнейшей обработки
    regfree(&reg);         //освобождение памяти
    fclose(file);          //закрытие файла
  }
}

int f_flag(char *pattern, char *filename) {  // обработка f флага
  FILE *file;                                //передаем файл
  file = fopen(filename, "r");               //читаем его
  int i = 0;
  if (file != NULL) {  //если файл есть
    int ch;
    while ((ch = fgetc(file)) != EOF) {  //берем символ пока он не !EOF
      if (ch == 13 || ch == 10) pattern[i++] = '|';  //добавляем
      if (ch != 13 && ch != 10) pattern[i++] = ch;   //добавляем
    }
  } else {
    printf("error\n");  //вывод ошибки если файл не открылся
    i = -1;             // ошибка
  }
  if (pattern[i - 1] ==
      '|')  // Проверяется, является ли последний символ в pattern символом '|'.
    pattern[i - 1] = '\0';  //Если да, то он заменяется на символ конца строки
                            //'\0', чтобы завершить строку pattern.
  fclose(file);  //закрываем файл для чтения
  return i;
}

void grep(struct flags *flags, int argc, char **argv, char *buff) {
  char pattern[BUFFER_SIZE] = {0};  //создаем буфер патернов
  int f_pattern = 0;
  if (!flags->f &&
      !flags->e)  //Если флаги f и e не установлены (!flags->f && !flags->e), то
                  //заполняем переменную pattern значением из аргумента
                  // argv[optind++] с помощью функции snprintf. Значение optind
                  //инкрементируется после присваивания.
    snprintf(pattern, BUFFER_SIZE, "%s", argv[optind++]);
  if (flags->f)
    f_pattern = f_flag(
        pattern,
        buff);  //(flags->f), вызываем функцию f_flag с аргументами pattern и
                // buff и присваиваем результат переменной f_patt
  if (!flags->f && flags->e)
    snprintf(pattern, BUFFER_SIZE, "%s",
             buff);  // (!flags->f && flags->e), заполняем переменную pattern
                     // значением из buff с помощью функции snprintf
  if (f_pattern != -1) {
    int file_true = 0;
    if (argc - optind > 1) file_true = 1;
    for (int i = optind; i < argc;
         i++) {  //Циклически перебираем элементы argv начиная с индекса optind
                 //и до конца массива argc
      if (file_true && !flags->h && !flags->l) printf("%s:", argv[i]);
      grep_file(flags, pattern,
                argv[i]);  //Вызываем функцию grep_file с аргументами flags,
                           // pattern и argv[i]
    }
  }
}