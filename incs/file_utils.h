#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "stdio.h"
#include "stdbool.h"
#include "archetype_fits.h"

typedef struct FileHandler FileHandler;

// Estructura para manejar el archivo
struct FileHandler {
    FILE* FP;
    char Mode[4];  // Almacena el modo de apertura ("r", "w", "a", etc.)
};

// Funciones para manejo de archivos
FileHandler* OpenFile(CharFixed, CharFixed, CharFixed);

Function CloseFile(FileHandler*);
Natural2 GetFileSize(FileHandler*);
bool FileExists(CharFixed, CharFixed);
bool FileTouch(CharFixed, CharFixed);
size_t ReadFile(FileHandler*, char*, size_t);
size_t WriteFile(FileHandler*, CharFixed, size_t);

#endif // FILE_UTILS_H