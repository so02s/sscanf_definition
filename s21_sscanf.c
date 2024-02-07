#include "s21_sscanf.h"

int s21_sscanf(const char *str, const char *format, ...) {
  int read_values = 0, error = 0, no_assig = 0;
  long read_char = 0;
  va_list ap;
  s_tok token;
  va_start(ap, format);
  char *form_ptr = (char *)format, *str_ptr = (char *)str;
  while (is_space(*form_ptr) && *form_ptr != '\0') form_ptr++;
  for (; *form_ptr != '\0' && !error;) {
    if (*form_ptr == '%') {
      form_ptr++;
      token = find_token(&form_ptr);
      if (token.spec) {
        if (token.no_assign == 0) no_assig++;
        if (token.spec != 'c')
          while (is_space(*str_ptr) && *str_ptr != '\0' && str_ptr++)
            read_char++;
        read_values += catch_arg(&str_ptr, token, ap, &read_char);
        while (is_space(*form_ptr) && *form_ptr != '\0') form_ptr++;
      } else
        error = 1;
    } else {
      if (*form_ptr == *str_ptr) {
        form_ptr++;
        str_ptr++;
        read_char++;
      } else {
        error = 1;
      }
    }
  }
  va_end(ap);
  if (!read_values && no_assig && token.spec && *str_ptr == '\0')
    return -1;
  else if (!read_char)
    return 0;
  else
    return read_values;
}

// =========== Поиск токенов ====================
s_tok find_token(char **format) {
  s_tok token;
  token.no_assign = 0;
  token.widht = 0;
  token.length = 0;
  token.spec = 0;
  if (**format == '*') {
    token.no_assign = 1;
    (*format)++;
  }
  if (!token.no_assign && is_number(**format)) do {
      token.widht = token.widht * 10 + **format - '0';
      (*format)++;
    } while (is_number(**format));
  if (is_length(**format)) {
    token.length = **format;
    (*format)++;
  }
  if (is_specif(**format)) {
    token.spec = **format;
    (*format)++;
  }
  return token;
}

// =========== Обработка аргументов ====================

#define SCAN_D(TYPE)                                                          \
  int scan_d_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, sign = numb_sing(str_ptr), read = 0;                     \
    sign = (sign) ? (++counter) * sign : 1;                                   \
    TYPE *arg = NULL, num = 0;                                                \
    if (!token.no_assign) arg = va_arg(ap, TYPE *);                           \
    while ((!token.widht || counter < token.widht) && is_number(**str_ptr)) { \
      if (!token.no_assign) {                                                 \
        num = num * 10 + sign * (**str_ptr - '0');                            \
        read = 1;                                                             \
      }                                                                       \
      counter++;                                                              \
      (*str_ptr)++;                                                           \
    }                                                                         \
    if (!token.no_assign && read) *arg = num;                                 \
    *n_count += counter;                                                      \
    return read;                                                              \
  }
#define SCAN_U(TYPE)                                                          \
  int scan_u_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, read = 0, sign = numb_sing(str_ptr);                     \
    sign = (sign) ? (++counter) * sign : 1;                                   \
    unsigned TYPE *arg = NULL, num = 0;                                       \
    if (!token.no_assign) arg = va_arg(ap, unsigned TYPE *);                  \
    while (is_number(**str_ptr) && (!token.widht || token.widht > counter)) { \
      if (!token.no_assign) {                                                 \
        num = num * 10 + sign * (**str_ptr - '0');                            \
        read = 1;                                                             \
      }                                                                       \
      counter++;                                                              \
      (*str_ptr)++;                                                           \
    }                                                                         \
    if (!token.no_assign && read) *arg = num;                                 \
    *n_count += counter;                                                      \
    return read;                                                              \
  }
#define SCAN_O(TYPE)                                                          \
  int scan_o_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, read = 0, sign = numb_sing(str_ptr);                     \
    unsigned TYPE *arg = NULL, num = 0;                                       \
    sign = (sign) ? (++counter) * sign : 1;                                   \
    if (!token.no_assign) arg = va_arg(ap, unsigned TYPE *);                  \
    while (is_oct_number(**str_ptr) &&                                        \
           (!token.widht || token.widht > counter)) {                         \
      if (!token.no_assign) {                                                 \
        num = num * 8 + sign * (**str_ptr - '0');                             \
        read = 1;                                                             \
      }                                                                       \
      counter++;                                                              \
      (*str_ptr)++;                                                           \
    }                                                                         \
    if (!token.no_assign && read) *arg = num;                                 \
    *n_count += counter;                                                      \
    return read;                                                              \
  }
#define SCAN_S(TYPE)                                                          \
  int scan_s_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, read = 0;                                                \
    TYPE *arg = NULL, ch = **str_ptr;                                         \
    if (!token.no_assign) arg = va_arg(ap, TYPE *);                           \
    while (!is_space(ch) && ch != '\0' &&                                     \
           (!token.widht || token.widht > counter)) {                         \
      if (!token.no_assign) {                                                 \
        *arg = ch;                                                            \
        arg++;                                                                \
        read = 1;                                                             \
      }                                                                       \
      counter++;                                                              \
      (*str_ptr)++;                                                           \
      ch = **str_ptr;                                                         \
    }                                                                         \
    if (!token.no_assign && read) *arg = '\0';                                \
    *n_count += counter;                                                      \
    return read;                                                              \
  }
#define SCAN_C(TYPE)                                                          \
  int scan_c_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, read = 0;                                                \
    TYPE *arg = NULL;                                                         \
    TYPE ch = **str_ptr;                                                      \
    if (!token.no_assign) arg = va_arg(ap, TYPE *);                           \
    if (token.widht)                                                          \
      while (ch != '\0' && token.widht > counter) {                           \
        if (!token.no_assign) {                                               \
          *arg = ch;                                                          \
          arg++;                                                              \
          read = 1;                                                           \
        }                                                                     \
        (*str_ptr)++;                                                         \
        ch = **str_ptr;                                                       \
        counter++;                                                            \
      }                                                                       \
    else if (ch != '\0' && !token.no_assign) {                                \
      *arg = ch;                                                              \
      (*str_ptr)++;                                                           \
      *n_count += 1;                                                          \
      read = 1;                                                               \
    }                                                                         \
    *n_count += counter;                                                      \
    return read;                                                              \
  }
#define SCAN_X(TYPE)                                                          \
  int scan_x_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, sign = numb_sing(str_ptr), read = 0;                     \
    sign = (sign) ? (++counter) * sign : 1;                                   \
    unsigned TYPE *arg = NULL, num = 0;                                       \
    if (!token.widht || token.widht > counter)                                \
      numb_pref(str_ptr, &counter, token, &read);                             \
    if (!token.no_assign) arg = va_arg(ap, unsigned TYPE *);                  \
    while ((!token.widht || token.widht > counter) &&                         \
           is_hex_number(**str_ptr)) {                                        \
      if (!token.no_assign) {                                                 \
        if (is_number(**str_ptr))                                             \
          num = num * 16 + sign * (**str_ptr - '0');                          \
        else if (**str_ptr >= 'a' && **str_ptr <= 'f')                        \
          num = num * 16 + sign * (**str_ptr - 'a' + 10);                     \
        else                                                                  \
          num = num * 16 + sign * (**str_ptr - 'A' + 10);                     \
        read = 1;                                                             \
      }                                                                       \
      (*str_ptr)++;                                                           \
      counter++;                                                              \
    }                                                                         \
    if (!token.no_assign && read) *arg = num;                                 \
    *n_count += counter;                                                      \
    return read;                                                              \
  }

#define SCAN_F(TYPE, TYPE2)                                                    \
  int scan_f_##TYPE2(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int count = 0, dot_count = 0, sign = numb_sing(str_ptr), read = 0;         \
    TYPE *arg = NULL, res = 0, fract_count = 1, exp = 0;                       \
    sign = (sign) ? (++count) * sign : 1;                                      \
    if (!token.no_assign) arg = va_arg(ap, TYPE *);                            \
    if (low_char(**str_ptr) == 'i' && low_char(*((*str_ptr) + 1)) == 'n' &&    \
        low_char(*((*str_ptr) + 2)) == 'f') {                                  \
      (*str_ptr) += 3;                                                         \
      count += 3;                                                              \
      res = sign * INFINITY;                                                   \
      read = 1;                                                                \
    } else if (low_char(**str_ptr) == 'n' &&                                   \
               low_char(*((*str_ptr) + 1)) == 'a' &&                           \
               low_char(*((*str_ptr) + 2)) == 'n') {                           \
      (*str_ptr) += 3;                                                         \
      count += 3;                                                              \
      res = NAN;                                                               \
      read = 1;                                                                \
    } else {                                                                   \
      char ch = **str_ptr;                                                     \
      while ((!token.widht || token.widht > count) && ch != '\0') {            \
        if (is_number(ch)) {                                                   \
          if (!dot_count)                                                      \
            res = res * 10 + sign * (ch - '0');                                \
          else {                                                               \
            fract_count /= 10;                                                 \
            res += fract_count * sign * (ch - '0');                            \
          }                                                                    \
          count++;                                                             \
          if (!token.no_assign) read = 1;                                      \
        } else if (ch == '.') {                                                \
          dot_count++;                                                         \
          count++;                                                             \
        } else if (ch == 'e' || ch == 'E') {                                   \
          (*str_ptr)++;                                                        \
          fract_count = 1;                                                     \
          dot_count = 0;                                                       \
          sign = numb_sing(str_ptr);                                           \
          ch = **str_ptr;                                                      \
          count++;                                                             \
          if (sign) {                                                          \
            count++;                                                           \
          } else                                                               \
            sign = 1;                                                          \
          while ((!token.widht || token.widht > count) && ch != '\0' &&        \
                 dot_count < 1) {                                              \
            if (is_number(ch)) {                                               \
              if (!dot_count)                                                  \
                exp = exp * 10 + (ch - '0');                                   \
              else {                                                           \
                fract_count /= 10;                                             \
                exp += fract_count * (ch - '0');                               \
              }                                                                \
            } else                                                             \
              dot_count = 2;                                                   \
            ch = *(++*str_ptr);                                                \
            count++;                                                           \
            if (!token.no_assign) read = 1;                                    \
          }                                                                    \
          res = res * powl(10, sign * exp);                                    \
        } else                                                                 \
          break;                                                               \
        if (dot_count > 1) break;                                              \
        ch = *(++*str_ptr);                                                    \
      }                                                                        \
    }                                                                          \
    if (!token.no_assign && read) *arg = res;                                  \
    *n_count += count;                                                         \
    return read;                                                               \
  }

#define SCAN_I(TYPE)                                                          \
  int scan_i_##TYPE(char **str_ptr, s_tok token, va_list ap, long *n_count) { \
    int counter = 0, sign = numb_sing(str_ptr), read = 0;                     \
    sign = (sign) ? (++counter) * sign : 1;                                   \
    TYPE *arg = NULL, num = 0;                                                \
    char typ = numb_pref(str_ptr, &counter, token, &read);                    \
    switch (typ) {                                                            \
      case 'd':                                                               \
        while (is_number(**str_ptr) &&                                        \
               (!token.widht || token.widht > counter)) {                     \
          if (!token.no_assign) {                                             \
            num = num * 10 + sign * (**str_ptr - '0');                        \
            (*n_count)++;                                                     \
            read = 1;                                                         \
          }                                                                   \
          (*str_ptr)++;                                                       \
          counter++;                                                          \
        }                                                                     \
        break;                                                                \
      case 'o':                                                               \
        while (is_oct_number(**str_ptr) &&                                    \
               (!token.widht || token.widht > counter)) {                     \
          if (!token.no_assign) {                                             \
            num = num * 8 + sign * (**str_ptr - '0');                         \
            (*n_count)++;                                                     \
            read = 1;                                                         \
          }                                                                   \
          (*str_ptr)++;                                                       \
          counter++;                                                          \
        }                                                                     \
        break;                                                                \
      case 'h':                                                               \
        while (is_hex_number(**str_ptr) &&                                    \
               (!token.widht || token.widht > counter)) {                     \
          if (!token.no_assign) {                                             \
            if (is_number(**str_ptr))                                         \
              num = num * 16 + sign * (**str_ptr - '0');                      \
            else if (**str_ptr >= 'a' && **str_ptr <= 'f')                    \
              num = num * 16 + sign * (**str_ptr - 'a' + 10);                 \
            else                                                              \
              num = num * 16 + sign * (**str_ptr - 'A' + 10);                 \
            (*n_count)++;                                                     \
            read = 1;                                                         \
          }                                                                   \
          (*str_ptr)++;                                                       \
          counter++;                                                          \
        }                                                                     \
    }                                                                         \
    if (!token.no_assign && read) {                                           \
      arg = va_arg(ap, TYPE *);                                               \
      *arg = num;                                                             \
    }                                                                         \
    *n_count += counter;                                                      \
    return read;                                                              \
  }

void scan_n(s_tok token, va_list ap, int n_count) {
  int *arg = NULL;
  if (!token.no_assign) {
    arg = va_arg(ap, int *);
    *arg = n_count;
  }
}

SCAN_D(int)
SCAN_D(long)
SCAN_D(short)
SCAN_U(int)
SCAN_U(long)
SCAN_U(short)
SCAN_O(int)
SCAN_O(long)
SCAN_O(short)
SCAN_S(char)
SCAN_S(wchar_t)
SCAN_C(char)
SCAN_C(wchar_t)
SCAN_X(short)
SCAN_X(int)
SCAN_X(long)
SCAN_F(float, float)
SCAN_F(double, double)
SCAN_F(long double, long)
SCAN_I(int)
SCAN_I(long)
SCAN_I(short)

    ;  //точка с запятой тк компилятор ругается

int scan_p(char **str_ptr, s_tok token, va_list ap, long *n_count) {
  int counter = 0, point = 0, read = 0;
  void **arg = NULL;
  numb_pref(str_ptr, &counter, token, &read);
  char ch = **str_ptr;
  while (is_hex_number(ch) && (!token.widht || token.widht > counter)) {
    if (!token.no_assign) {
      if (is_number(ch))
        point = point * 16 + (ch - '0');
      else if (ch >= 'a' && ch <= 'f')
        point = point * 16 + (ch - 'a' + 10);
      else
        point = point * 16 + (ch - 'A' + 10);
      read = 1;
    }
    (*str_ptr)++;
    ch = **str_ptr;
    counter++;
  }
  if (!token.no_assign) {
    arg = va_arg(ap, void **);
    if (point == 0)
      *arg = NULL;
    else
      *arg = (void *)(uintptr_t)point;  // <==== может не собраться из-за этого
  }
  *n_count += counter;
  return read;
}

void scan_proc(char **str_ptr, s_tok token, long *n_count) {
  int counter = 0;
  if (!token.widht && **str_ptr == '%') {
    (*str_ptr)++;
    counter++;
  } else if (token.widht) {
    while (token.widht > counter && **str_ptr == '%') {
      (*str_ptr)++;
      counter++;
    }
  }
  *n_count += counter;
}

// ==================== штука для выбора спецификатора и ширины ================

typedef int (*func)(char **, s_tok, va_list, long *);
int use_f(func d, func L, func l, func h, char **str, s_tok tk, va_list ap,
          long *count) {
  int read = 0;
  if (tk.length == 'L' && L != NULL)
    read = L(str, tk, ap, count);
  else if (tk.length == 'l' && l != NULL)
    read = l(str, tk, ap, count);
  else if (tk.length == 'h' && h != NULL)
    read = h(str, tk, ap, count);
  else
    read = d(str, tk, ap, count);
  return read;
}
int catch_arg(char **str, s_tok tk, va_list ap, long *cnt) {
  int res = 0;
  switch (tk.spec) {
    case 'd':
      res =
          use_f(scan_d_int, NULL, scan_d_long, scan_d_short, str, tk, ap, cnt);
      break;
    case 'u':
      res =
          use_f(scan_u_int, NULL, scan_u_long, scan_u_short, str, tk, ap, cnt);
      break;
    case 'o':
      res =
          use_f(scan_o_int, NULL, scan_o_long, scan_o_short, str, tk, ap, cnt);
      break;
    case 's':
      res = use_f(scan_s_char, scan_s_wchar_t, NULL, NULL, str, tk, ap, cnt);
      break;
    case 'c':
      res = use_f(scan_c_char, scan_c_wchar_t, NULL, NULL, str, tk, ap, cnt);
      break;
    case 'x':
    case 'X':
      res =
          use_f(scan_x_int, NULL, scan_x_long, scan_x_short, str, tk, ap, cnt);
      break;
    case 'f':
    case 'e':
    case 'E':
    case 'g':
    case 'G':
      res = use_f(scan_f_float, scan_f_long, scan_f_double, NULL, str, tk, ap,
                  cnt);
      break;
    case 'i':
      res =
          use_f(scan_i_int, NULL, scan_i_long, scan_i_short, str, tk, ap, cnt);
      break;
    case 'p':
      res = scan_p(str, tk, ap, cnt);
      break;
    case '%':
      scan_proc(str, tk, cnt);
      break;
    case 'n':
      scan_n(tk, ap, *cnt);
  }
  return res;
}

// ================ Вспомогательные функции =====================

int is_specif(char ch) {
  return ch == 'c' || ch == 'd' || ch == 'i' || ch == 'e' || ch == 'E' ||
         ch == 'f' || ch == 'g' || ch == 'G' || ch == 'o' || ch == 's' ||
         ch == 'u' || ch == 'x' || ch == 'X' || ch == 'p' || ch == 'n' ||
         ch == '%';
}
int is_length(char ch) { return ch == 'h' || ch == 'l' || ch == 'L'; }
int is_number(char ch) { return ch >= '0' && ch <= '9'; }
int is_oct_number(char ch) { return ch >= '0' && ch <= '7'; }
int is_hex_number(char ch) {
  return is_number(ch) || (up_char(ch) >= 'A' && up_char(ch) <= 'F');
}
int is_space(int ch) { return ((ch >= 9 && ch <= 13) || ch == 32); }
int numb_sing(char **ch) {
  int res = 0;
  if (**ch == '-') {
    res = -1;
    (*ch)++;
  } else if (**ch == '+') {
    res = 1;
    (*ch)++;
  }
  return res;
}
char up_char(char ch) { return ch >= 'a' && ch <= 'z' ? ch - ('a' - 'A') : ch; }
char low_char(char ch) {
  return ch >= 'A' && ch <= 'Z' ? ch + ('a' - 'A') : ch;
}
char numb_pref(char **ch, int *counter, s_tok token, int *read) {
  char res = 'd';
  if (!token.widht || token.widht > *counter)
    if (**ch == '0') {
      (*ch)++;
      res = 'o';
      (*counter)++;
      if (!token.no_assign) *read = 1;
      if (!token.widht || token.widht > *counter)
        if (**ch == 'x' || **ch == 'X') {
          res = 'h';
          (*ch)++;
          (*counter)++;
        }
    }
  return res;
}