#ifndef FILEMAN_H
#define FILEMAN_H

#include "easy_cody.h"

typedef struct FM* FM;

// Estructura para manejar el archivo
struct FM {
    FILE* FP;
    char Mode[4];  // Almacena el modo de apertura ("r", "w", "a", etc.)
};

// Funciones para manejo de archivos
FM OpenFile(str Dir, str Name, str Mode);
void CloseFile(FM File);
uint GetFileSize(FM File);
bool FileExists(str Dir, str Name);
bool FileTouch(str Dir, str Name);
size_t ReadFile(FM File, void* Buffer, size_t Size, size_t Count);
size_t WriteFile(FM File, const void* Buffer, size_t Size, size_t Count);

#endif // FILEMAN_H