#include "s21_sscanf.h"

int s21_sscanf(const char* str, const char *format, ...){
    int error = 0;
    va_list ap;
    s_tok token[10]; //задать не статически
    va_start(ap, format);
    // сначала - убрать пробелы, табы и тд
    format = delete_whitespaces(format);
    // далее - отформатировать строку format на токены
    // error = find_tokens(format, token);
    if(error){
        printf("ERROR");
        va_end(ap);
        free((void*)format);
        return 1;
    }
    // потом по порядку искать i-ый токен с начала строки str
    for(int i = 0; ; i++){ //тут длинна листа аргументов
        // и связывать его с i-ым адресом (сначала - проверить тип у адреса)
        error = (token[i].spec == sizeof(va_arg(ap, char))) ? 1 : 0; // <========= !!!!!!!!
        if(error){
            printf("ERROR");
            va_end(ap);
            free((void*)format);
            return 1;
        }
        // else
            // find_coincid_str(str, token[i], arg); //последнее - это адре
    }
    va_end(ap);
    free((void*)format);
    return 0;
}

char* delete_whitespaces(const char *format) {
    int i = 0, j = 0;
    char *new_format = malloc(sizeof(char));
    //пробежка по всей строке циклом
    for (; format[i] != '\0'; i++)
        if(!s21_isspace(format[i])){
            new_format = realloc(new_format, (i+2)*sizeof(char));
            new_format[j++] = format[i];
        }
    new_format[j] = '\0';
    return new_format;
}

// int find_tokens(char *format, s_tok *token) {
//     return 0;
// }

// int there_no_errors(s_tok token, arg) {
    
// }

// void find_coincid_str(char *str, s_tok token, arg) {
    
// }

int s21_isspace(int ch){
    if((ch >= 9 && ch <= 13) || ch == 32)
        return 1;
    else
        return 0;
}

int main(){
    const char *f = "Hello  Word and something else      ... fwsdf    §§fwf wefw";
    printf("before = %s\n", f);
    f = delete_whitespaces(f);
    printf("after = %s", f);
    free((void*)f);
}