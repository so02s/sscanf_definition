#include "s21_sscanf.h"

int s21_sscanf(const char* str, const char *format, ...){
    //int error = 0;
    va_list ap;
    s_tok token;
    va_start(ap, format);
    char *form_ptr = (char*)format, *str_ptr = (char*)str;
    // сравнивать str и format
    // format не смотрит на пробелы 
    for(; *form_ptr != '\0'; form_ptr++){
        if(*form_ptr == '%'){
            form_ptr++;
            token = find_token(&form_ptr);
            // printf("token = %d %d %c %c",token.no_assign, token.widht,
            //     token.length, token.spec);
            }
        if(token.spec)
            catch_arg(&str_ptr, token, va_arg(ap, void*));
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

int catch_arg(char **str_ptr, s_tok token, void* arg){
    if(token.spec == 'd'){
        int *p = (int*)arg;
        *p = 0;
        char ch = **str_ptr;
        while(ch >= '0' && ch <= '9'){
            *p = (*p) ? *p * 10 + (ch - 48) : (ch - 48);
            (*str_ptr)++;
            ch = **str_ptr;
        }
    }
    return 0;
}

s_tok find_token(char **format) {
    s_tok token;
    char ch = **format;
    token.no_assign = 0;
    token.widht = 0;
    token.length = 0;
    token.spec = 0;
    if(ch == '*'){
        token.no_assign = 1;
        (*format)++;
    }
    if(is_length(**format)){
        token.length = **format;
        (*format)++;
    }
    if(is_specif(**format))
        token.spec = **format;
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

void some_test(size_t num, ...){
    va_list ap;
    void* ptr = NULL;
    va_start(ap,num);
    for (size_t i = 0; i < num; i++){
        ptr = va_arg(ap,void*);
        *((int*)ptr) += 10;
        printf("%d\n",*((int*)ptr));
    }
}

void scanf_test(){
    int t = 33, k = 84;
    s21_sscanf("12324rew3", "%d", &t);
    printf("%d", t);
}

int main(){
    scanf_test();
}

