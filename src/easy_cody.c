#include "easy_cody.h"

bool DebugMode = true;

void Msg(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);  // Función para imprimir con varargs
    va_end(args);
    printf("\n");
}

void Failure(const char *format, ...) {
    printf("FATAL FAILURE: ");
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
    abort();
}

bool DebugStatus(){
    return DebugMode;
}

void Debug(const char *format, ...) {
    if (DebugStatus()) {
        printf("DEBUG: ");
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
    }
}