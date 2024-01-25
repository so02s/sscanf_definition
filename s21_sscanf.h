#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// %[*][ширина][длина]спецификатор
typedef struct s_token {
  int no_assign;
  int widht;
  char length;
  char spec;
} s_tok;

int s21_sscanf(const char* str, const char *format, ...);
char* delete_whitespaces(const char *format);
s_tok find_token(char **format);
// int there_no_errors(s_tok token, arg); //????
// void find_coincid_str(char *str, s_tok token, arg);
int s21_isspace(int ch);
int is_specif(char ch);
int is_length(char ch);
int catch_arg(char **str_ptr, s_tok token, void* arg);