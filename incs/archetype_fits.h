#ifndef ARCHETYPE_FITS_H
#define ARCHETYPE_FITS_H

#include <stdlib.h>
#include <stdbool.h>

#define Max(a, b) ((a > b) ? a : b)
#define Min(a, b) ((a < b) ? a : b)
#define Frac(a,b) ((Decimal)(a)/(Decimal)(b))
#define Loop while(1)
#define Repeat(n) for(Integer2 i=0; i<n; i++)

//Retornos
#define End return 0;
#define FailEnd return -1;

// Gestor de mensajes
#define Msg(format, ...) { \
    printf(format, ##__VA_ARGS__); \
    printf("\n"); \
}

#define Failure(format, ...) { \
    printf("FATAL FAILURE: "); \
    Msg(format, ##__VA_ARGS__); \
    abort(); \
}

#define Debug(format, ...) { \
    if (DebugMode) { \
        printf("DEBUG: "); \
        Msg(format, ##__VA_ARGS__); \
    } \
}

typedef void Function;
typedef void* ElemType;
typedef unsigned short Natural;
typedef short Integer;
typedef unsigned int Natural2;
typedef int Integer2;
typedef double Decimal;
typedef const char* CharFixed;

#endif // ARCHETYPE_FITS_H