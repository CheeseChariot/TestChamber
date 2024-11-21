#ifndef EASY_CODY_H
#define EASY_CODY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define Max(a, b) ((a > b) ? a : b)
#define Min(a, b) ((a < b) ? a : b)
#define Frac(a,b) ((Decimal)(a)/(Decimal)(b))
#define Loop while(1)
#define Repeat(n) for(Integer2 i=0; i<n; i++)

//Tipos
#define uint unsigned int
#define ushort unsigned short
#define uchar unsigned char
#define ulong unsigned long
#define ullong unsigned long long
#define str char*

//Retornos
#define End return 0;
#define FailEnd return -1;

//Gestor de mensajes
extern bool DebugMode = false;
void Msg(const char *format, ...);
void Failure(const char *format, ...);
void Debug(const char *format, ...);

#endif // ARCHETYPE_FITS_H