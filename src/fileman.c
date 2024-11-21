#include "fileman.h"

FM OpenFile(str Dir, str Name, str Mode) {
    Debug("OpenFile: Intentando abrir archivo...");
    Debug("OpenFile: D-> %s | A-> %s | M-> %s",Dir,Name,Mode);
    FM File = (FM) malloc(sizeof(struct FM));
    if (File == NULL) {
        Debug("OpenFile: Error al asignar memoria para el archivo.");
        return NULL;
    }

    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", Dir, Name);
    Debug("OpenFile: Ruta completa del archivo generada.");

    File->FP = fopen(FilePath, Mode);
    if (File->FP == NULL) {
        Debug("OpenFile: No se pudo abrir el archivo.");
        free(File);
        return NULL;
    }

    snprintf(File->Mode, sizeof(File->Mode), "%s", Mode);
    Debug("OpenFile: Archivo abierto exitosamente en modo '%s'.", Mode);

    return File;
}

void CloseFile(FM File) {
    Debug("CloseFile: Intentando cerrar archivo...");
    if (File && File->FP) {
        fclose(File->FP);
        Debug("CloseFile: Archivo cerrado.");
        free(File);
        Debug("CloseFile: Memoria liberada.");
    } else {
        Debug("CloseFile: Archivo ya estaba cerrado o no válido.");
    }
}

uint GetFileSize(FM File) {
    Debug("GetFileSize: Calculando tamaño del archivo...");
    if (File == NULL || File->FP == NULL) {
        Debug("GetFileSize: Archivo no válido o no abierto.");
        return 0;
    }

    long CurrentPos = ftell(File->FP);
    fseek(File->FP, 0, SEEK_END);
    long Size = ftell(File->FP);
    fseek(File->FP, CurrentPos, SEEK_SET);
    Debug("GetFileSize: Tamaño del archivo calculado: %lu bytes.", Size);

    return (uint) Size;
}

bool FileExists(str Dir, str Name) {
    Debug("FileExists: Comprobando si el archivo existe...");
    FM Temp = OpenFile(Dir, Name, "r");
    if (Temp && Temp->FP) {
        CloseFile(Temp);
        Debug("FileExists: Archivo encontrado.");
        return true;
    }
    Debug("FileExists: Archivo no encontrado.");
    return false;
}

bool FileTouch(str Dir, str Name) {
    Debug("FileTouch: Intentando crear archivo vacío...");
    FM Temp = OpenFile(Dir, Name, "a");
    if (Temp && Temp->FP) {
        CloseFile(Temp);
        Debug("FileTouch: Archivo creado exitosamente.");
        return true;
    }
    Debug("FileTouch: No se pudo crear el archivo.");
    return false;
}

size_t ReadFile(FM File, void* Buffer, size_t Size, size_t Count) {
    Debug("ReadFile: Intentando leer archivo...");
    if (File == NULL || File->FP == NULL) {
        Debug("ReadFile: Archivo no válido o no abierto.");
        return 0;
    }
    if (Buffer == NULL) {
        Debug("ReadFile: Buffer de lectura es nulo.");
        return 0;
    }
    if (strchr(File->Mode, 'r') == NULL && strchr(File->Mode, '+') == NULL) {
        Debug("ReadFile: Archivo no está abierto en modo lectura.");
        return 0;
    }
    size_t ReadCount = fread(Buffer, Size, Count, File->FP);
    Debug("ReadFile: Se leyeron %zu bloques de tamaño %zu.", ReadCount, Size);
    return ReadCount;
}

size_t WriteFile(FM File, const void* Buffer, size_t Size, size_t Count) {
    Debug("WriteFile: Intentando escribir en archivo...");
    if (File == NULL || File->FP == NULL) {
        Debug("WriteFile: Archivo no válido o no abierto.");
        return 0;
    }
    if (Buffer == NULL) {
        Debug("WriteFile: Buffer de escritura es nulo.");
        return 0;
    }
    if (strchr(File->Mode, 'w') == NULL && strchr(File->Mode, 'a') == NULL && strchr(File->Mode, '+') == NULL) {
        Debug("WriteFile: Archivo no está abierto en modo escritura o anexado.");
        return 0;
    }
    size_t WriteCount = fwrite(Buffer, Size, Count, File->FP);
    Debug("WriteFile: Se escribieron %zu bloques de tamaño %zu.", WriteCount, Size);
    return WriteCount;
}
