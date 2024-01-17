typedef struct s_token {
  int no_assign;
  int widht;
  char length;
  char spec;
} s_tok;

int s21_sscanf(const char* str, const char *format, ...);
void delete_whitespaces(char *format);
int find_tokens(char *format, s_tok *token);
int there_no_errors(s_tok token, arg); //????
void find_coincid_str(char *str, s_tok token, arg);