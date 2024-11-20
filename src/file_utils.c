#include "file_utils.h"
#include <stdlib.h>
#include "archetype_fits.h"

// Abre el archivo y devuelve un puntero a FileHandler
FileHandler* OpenFile(CharFixed FolderPath, CharFixed FileName, CharFixed Mode) {
    FileHandler* File = (FileHandler *) malloc(sizeof(FileHandler));
    if (File == NULL) return NULL;

    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", FolderPath, FileName);

    File->FP = fopen(FilePath, Mode);

    if (File->FP == NULL) {
        free(File);
        return NULL;
    }

    snprintf(File->Mode, sizeof(File->Mode), "%s", Mode);

    return File;
}

// Cierra el archivo y libera memoria
Function CloseFile(FileHandler* File) {
    if (File != NULL) {
        if (File->FP != NULL) {
            fclose(File->FP);
        }

        free(File);
    }

    return;
}

// Obtiene el tamaño del archivo en bytes
Natural2 GetFileSize(FileHandler* File) {
    if (File == NULL || File->FP == NULL) {
        return 0;
    }

    long CurrentPos = ftell(File->FP);

    fseek(File->FP, 0, SEEK_END);

    size_t size = ftell(File->FP);

    fseek(File->FP, CurrentPos, SEEK_SET);

    return size;
}

// Verifica si un archivo existe
bool FileExists(CharFixed FolderPath, CharFixed FileName) {

    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", FolderPath, FileName);

    FILE *File = fopen(FilePath, "r");

    if (File) {
        fclose(File);
        return true;
    }

    return false;
}

//Crea el archivo o actualiza marca de existencia.
bool FileTouch(CharFixed FolderPath, CharFixed FileName) {
    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", FolderPath, FileName);

    FILE *File = fopen(FilePath, "a"); // Usa el modo "a" para abrir o crear el archivo
    if (File) {
        fclose(File);
        return true; // Operación exitosa
    }

    return false; // Error al intentar tocar el archivo
}

// Lee del archivo y almacena en buffer hasta un máximo de 'size' bytes
size_t ReadFile(FileHandler* File, char* Buffer, size_t Size) {
    if (File == NULL || File->FP == NULL) {
        return 0;
    }

    return fread(Buffer, 1, Size, File->FP);
}

// Escribe en el archivo desde el buffer hasta un máximo de 'size' bytes
size_t WriteFile(FileHandler* File, CharFixed Buffer, size_t Size) {
    if (File == NULL || File->FP == NULL) {
        return 0;
    }

    return fwrite(Buffer, 1, Size, File->FP);
}