#include "s21_sscanf.h"

int s21_sscanf(const char* str, const char *format, ...){
    int error = 0;
    s_tok token[10]; //задать не статически
    // сначала - убрать пробелы, табы и тд
    delete_whitespaces(format);
    // далее - отформатировать строку format на токены
    error = find_tokens(format, token);
    if(error){ // неверный ввод
        printf("ERROR");
        return 1;
    }
    // потом по порядку искать i-ый токен с начала строки str
    for(int i = 0; i < ?; i++){ //тут длинна листа аргументов
        // и связывать его с i-ым адресом (сначала - проверить тип у адреса)
        error = there_no_errors(token[i], arg);
        if(error){
            printf("ERROR");
            return 1;
        }
        else
            find_coincid_str(str, token[i], arg); //последнее - это адре
    }
    return 0;
}