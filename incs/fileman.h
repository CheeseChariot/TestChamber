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

#endif // FILEMAN_H