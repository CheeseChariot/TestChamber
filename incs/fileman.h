#ifndef FILEMAN_H
#define FILEMAN_H

#include "easy_cody.h"
#include <dirent.h>
#include <unistd.h>


typedef struct FM* FM;

// Estructura para manejar el archivo
struct FM {
    FILE* FP;
    char Path[256];
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

// Navegación de directorios
bool ChangeDirectory(str Dir);
char* GetCurrentDirectory();
bool GoToParentDirectory();
bool GoToRootDirectory();
str* ListDirectory(str Dir, size_t* Count);
str* ListCurrentDirectory(size_t* Count);

// Funciones auxiliares
void PrintFileContentWithWrap(FM File, size_t LineWidth);
str CombinePath(str Dir, str Name);
void FreeFileList(str* FileList, size_t Count);

//Función de Ejemplo o Test
void FilemanTest();

#endif // FILEMAN_H