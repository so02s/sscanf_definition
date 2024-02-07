#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define PRECISION 1e-5

// %[*][ширина][длина]спецификатор
typedef struct s_token {
  int no_assign;
  int widht;
  char length;
  char spec;
} s_tok;

int s21_sscanf(const char *str, const char *format, ...);
s_tok find_token(char **format);
int catch_arg(char **str_ptr, s_tok token, va_list ap, long *n_count);
void scan_n(s_tok token, va_list ap, int n_count);
int scan_p(char **str_ptr, s_tok token, va_list ap,
           long *n_count);  // <====могут быть ошибки при сборке
void scan_proc(char **str_ptr, s_tok token, long *n_count);
int is_specif(char ch);
int is_length(char ch);
int is_number(char ch);
int is_oct_number(char ch);
int is_hex_number(char ch);
int is_space(int ch);
int numb_sing(char **ch);
char up_char(char ch);
char low_char(char ch);
char numb_pref(char **ch, int *counter, s_tok token, int *readed);
 