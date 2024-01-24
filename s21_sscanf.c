#include "s21_sscanf.h"

int s21_sscanf(const char* str, const char *format, ...){
    //int error = 0;
    va_list ap;
    s_tok token;
    va_start(ap, format);
    const char *ptr = format;
    const char *str_ptr = str;
    // сначала - убрать пробелы, табы и тд
    // format = delete_whitespaces(format); // возможно это не нужно !!!!!!!!!!!!
    // далее - отформатировать строку format на токены
    // error = find_tokens(format, token);
    // if(error){
    //     printf("ERROR");
    //     va_end(ap);
    //     free((void*)format);
    //     return 1;
    // }
    // сравнивать str и format
    for(char ch = *ptr; ch != '\0'; ){
        if(ch == '%')
            token = find_token(&ptr);
        printf("HELL");
        if(token.spec)
            catch_arg(&str_ptr, token, va_arg(ap, void*));
        ch = *(++ptr);
        // error = (token[i].spec == sizeof(va_arg(ap, char))) ? 1 : 0; // <========= !!!!!!!!
        // if(error){
        //     printf("ERROR");
        //     va_end(ap);
        //     free((void*)format);
        //     return 1;
        // }
        // else
            // find_coincid_str(str, token[i], arg);
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

int catch_arg(const char **str_ptr, s_tok token, void* arg){
    if(token.spec == 'd'){
        *((int*)arg) = 0;
        while(**str_ptr >= '0' && **str_ptr <= '9'){
            *((int*)arg) = ( *((int*)arg)) ? *((int*)arg)*10+**str_ptr : **str_ptr;
            (*str_ptr)++;
        }
    }
    return 0;
}

s_tok find_token(const char **format) {
    s_tok token;
    token.no_assign = 0;
    token.widht = 0;
    token.length = 0;
    token.spec = 0;
    if(**format == '*'){
        token.no_assign = 1;
        (*format)++;
    }
    if(is_length(**format)){
        token.length = **format;
        (*format)++;
    }
    if(is_specif(**format)){
        token.spec = **format;
        (*format)++;
    }
    return token;
}

int is_length(char ch){
    return ch == 'h' || ch == 'l' || ch == 'L';
}

int is_specif(char ch){
    return  ch == 'c' || ch == 'd' ||
            ch == 'i' || ch == 'e' ||
            ch == 'E' || ch == 'f' ||
            ch == 'g' || ch == 'G' ||
            ch == 'o' || ch == 's' ||
            ch == 'u' || ch == 'x' ||
            ch == 'X' || ch == 'p' ||
            ch == 'n';
}

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
    int t = 0;
    s21_sscanf("123", "%d", &t);
    printf("%d", t);
}