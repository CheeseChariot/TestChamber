#include "fileman.h"

FM OpenFile(str Dir, str Name, str Mode) {
    Debug("OpenFile: Intentando abrir archivo...");
    Debug("OpenFile: D-> %s | A-> %s | M-> %s", Dir, Name, Mode);
    FM File = (FM) malloc(sizeof(struct FM));
    if (File == NULL) {
        Failure("OpenFile: Error al asignar memoria para el archivo.");
        return NULL;
    }

    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", Dir, Name);
    strcpy(File->Path, FilePath);
    Debug("OpenFile: Ruta completa del archivo generada.");

    File->FP = fopen(FilePath, Mode);
    if (File->FP == NULL) {
        Failure("OpenFile: No se pudo abrir el archivo.");
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

bool ChangeDirectory(str Dir) {
    Debug("ChangeDirectory: Intentando cambiar al directorio '%s'.", Dir);
    if (chdir(Dir) == 0) {
        Debug("ChangeDirectory: Directorio cambiado exitosamente a '%s'.", Dir);
        return true;
    } else {
        Debug("ChangeDirectory: No se pudo cambiar al directorio '%s'.", Dir);
        return false;
    }
}

char* GetCurrentDirectory() {
    Debug("GetCurrentDirectory: Obteniendo el directorio actual...");
    char* CurrentDir = malloc(256);
    if (!CurrentDir) {
        Failure("GetCurrentDirectory: No se pudo asignar memoria.");
        return NULL;
    }

    if (getcwd(CurrentDir, 256) != NULL) {
        Debug("GetCurrentDirectory: Directorio actual es '%s'.", CurrentDir);
        return CurrentDir;
    } else {
        Failure("GetCurrentDirectory: No se pudo obtener el directorio actual.");
        free(CurrentDir);
        return NULL;
    }
}

bool GoToParentDirectory() {
    Debug("GoToParentDirectory: Intentando ir al directorio padre...");
    if (ChangeDirectory("..")) {
        Debug("GoToParentDirectory: Cambiado al directorio padre.");
        return true;
    } else {
        Debug("GoToParentDirectory: No se pudo cambiar al directorio padre.");
        return false;
    }
}

bool GoToRootDirectory() {
    Debug("GoToRootDirectory: Intentando ir al directorio raíz...");
    if (ChangeDirectory("/")) {
        Debug("GoToRootDirectory: Cambiado al directorio raíz.");
        return true;
    } else {
        Debug("GoToRootDirectory: No se pudo cambiar al directorio raíz.");
        return false;
    }
}

str* ListDirectory(str Dir, size_t* Count) {
    Debug("ListDirectory: Listando archivos en el directorio '%s'.", Dir);
    DIR* Directory = opendir(Dir);
    if (!Directory) {
        Failure("ListDirectory: No se pudo abrir el directorio '%s'.", Dir);
        return NULL;
    }

    struct dirent* Entry;
    size_t Capacity = 10;
    *Count = 0;

    str* FileList = malloc(Capacity * sizeof(char*));
    if (!FileList) {
        Failure("ListDirectory: No se pudo asignar memoria para la lista de archivos.");
        closedir(Directory);
        return NULL;
    }

    while ((Entry = readdir(Directory)) != NULL) {
        if (*Count == Capacity) {
            Capacity *= 2;
            str* Temp = realloc(FileList, Capacity * sizeof(char*));
            if (!Temp) {
                Failure("ListDirectory: No se pudo redimensionar la lista de archivos.");
                for (size_t i = 0; i < *Count; i++) free(FileList[i]);
                free(FileList);
                closedir(Directory);
                return NULL;
            }
            FileList = Temp;
        }

        FileList[*Count] = strdup(Entry->d_name);
        Debug("ListDirectory: Archivo encontrado: %s", Entry->d_name);
        (*Count)++;
    }

    closedir(Directory);
    Debug("ListDirectory: Se encontraron %zu archivos.", *Count);
    return FileList;
}

str* ListCurrentDirectory(size_t* Count) {
    Debug("ListCurrentDirectory: Listando archivos en el directorio actual...");
    char* CurrentDir = GetCurrentDirectory();
    if (!CurrentDir) {
        Failure("ListCurrentDirectory: No se pudo obtener el directorio actual.");
        return NULL;
    }

    str* FileList = ListDirectory(CurrentDir, Count);
    free(CurrentDir);
    return FileList;
}

void PrintFileContentWithWrap(FM File, size_t LineWidth) {
    if (File == NULL || File->FP == NULL) {
        Failure("PrintFileContentWithWrap: Archivo no válido.");
        return;
    }

    char buffer[1024];
    size_t len = 0;
    char* line = NULL;
    size_t line_len = 0;

    while (fgets(buffer, sizeof(buffer), File->FP)) {
        size_t buffer_len = strlen(buffer);
        size_t index = 0;

        while (index < buffer_len) {
            size_t next_space = index + LineWidth;
            if (next_space > buffer_len) {
                next_space = buffer_len;
            }

            while (next_space > index && buffer[next_space] != ' ' && next_space > index) {
                next_space--;
            }

            if (next_space == index) {
                next_space = index + LineWidth;
            }

            for (size_t i = index; i < next_space; i++) {
                putchar(buffer[i]);
            }
            putchar('\n');

            index = next_space + 1;
        }
    }
}

str CombinePath(str Dir, str Name) {
    size_t len = strlen(Dir) + strlen(Name) + 2;
    str Path = malloc(len);
    if (Path == NULL) {
        Failure("CombinePath: Error al asignar memoria para la ruta.");
        return NULL;
    }
    snprintf(Path, len, "%s/%s", Dir, Name);
    return Path;
}

void FreeFileList(str* FileList, size_t Count) {
    for (size_t i = 0; i < Count; i++) {
        free(FileList[i]);
    }
    free(FileList);
}

void FilemanExample() {
    str tempDir = "/tmp";
    size_t count;
    
    if (!ChangeDirectory(tempDir)) {
        Debug("FilemanExample: No se pudo cambiar al directorio temporal.");
        return;
    }
    
    FM file = OpenFile(tempDir, "example.txt", "w");
    if (file == NULL) {
        Debug("FilemanExample: No se pudo abrir el archivo.");
        return;
    }

    const char* content = "Este es un ejemplo de contenido para el archivo. "
                          "Este texto debe ser mostrado correctamente en varias líneas "
                          "dependiendo del ancho especificado para el salto de línea automático.";
    WriteFile(file, content, 1, strlen(content));

    CloseFile(file);

    str* files = ListDirectory(tempDir, &count);
    if (files == NULL) {
        Debug("FilemanExample: No se pudo listar los archivos del directorio temporal.");
        return;
    }

    Debug("FilemanExample: Archivos encontrados en %s:", tempDir);
    for (size_t i = 0; i < count; i++) {
        Debug("FilemanExample: %s", files[i]);
        free(files[i]);
    }
    free(files);

    if (!GoToParentDirectory()) {
        Debug("FilemanExample: No se pudo volver al directorio anterior.");
        return;
    }

    file = OpenFile(tempDir, "example.txt", "r");
    if (file == NULL) {
        Debug("FilemanExample: No se pudo volver a abrir el archivo.");
        return;
    }

    Debug("FilemanExample: Mostrando contenido del archivo con salto de línea automático:");
    PrintFileContentWithWrap(file, 40);
    CloseFile(file);
}