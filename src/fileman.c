#include "fileman.h"

// Funciones para manejo de archivos

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

size_t WriteFile(FM File, const void* Buffer, size_t Size, size_t Count) {
    if (File == NULL || File->FP == NULL) {
        Failure("WriteFile: Archivo no válido.");
        return 0;
    }
    return fwrite(Buffer, Size, Count, File->FP);
}

size_t ReadFile(FM File, void* Buffer, size_t Size, size_t Count) {
    if (File == NULL || File->FP == NULL) {
        Failure("ReadFile: Archivo no válido.");
        return 0;
    }
    return fread(Buffer, Size, Count, File->FP);
}

uint GetFileSize(FM File) {
    if (File == NULL || File->FP == NULL) {
        Failure("GetFileSize: Archivo no válido.");
        return 0;
    }

    fseek(File->FP, 0, SEEK_END);
    uint size = ftell(File->FP);
    fseek(File->FP, 0, SEEK_SET);

    return size;
}

bool FileExists(str Dir, str Name) {
    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", Dir, Name);

    FILE* File = fopen(FilePath, "r");
    if (File) {
        fclose(File);
        return true;
    }

    return false;
}

bool FileTouch(str Dir, str Name) {
    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", Dir, Name);

    FILE* File = fopen(FilePath, "a");
    if (File) {
        fclose(File);
        return true;
    }

    return false;
}

// Navegación de directorios

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

// Funciones auxiliares

void PrintFileContentWithWrap(FM File, size_t LineWidth) {
    if (File == NULL || File->FP == NULL) {
        Failure("PrintFileContentWithWrap: Archivo no válido.");
        return;
    }

    char buffer[1024];
    size_t index = 0;

    while (fgets(buffer, sizeof(buffer), File->FP)) {
        size_t buffer_len = strlen(buffer);

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
    char* Combined = malloc(strlen(Dir) + strlen(Name) + 2);
    if (!Combined) {
        Failure("CombinePath: Error al asignar memoria para la ruta combinada.");
        return NULL;
    }

    snprintf(Combined, strlen(Dir) + strlen(Name) + 2, "%s/%s", Dir, Name);
    return Combined;
}

void FreeFileList(str* FileList, size_t Count) {
    for (size_t i = 0; i < Count; i++) {
        free(FileList[i]);
    }
    free(FileList);
}

//Función de Ejemplo o Test

void FilemanTest() {
    if (DebugStatus()) {
        // Probar la creación y apertura de un archivo
        str dir = malloc(256);
        strcpy(dir, ".");

        str name = malloc(256);
        strcpy(name, "test.txt");

        str mode = malloc(256);
        strcpy(mode, "w");


        FM file = OpenFile(dir, name, mode);
        if (file != NULL) {
            Msg("Fileman Test: Archivo abierto con éxito.");
            
            // Probar escritura en el archivo
            const char* text = "Esto es una prueba de escritura en un archivo.";
            size_t bytes_written = WriteFile(file, text, sizeof(char), strlen(text));
            Msg("Fileman Test: Escritos %zu bytes en el archivo.", bytes_written);
            
            // Cerrar el archivo
            CloseFile(file);
        }

        // Probar si el archivo existe
        if (FileExists(dir, name)) {
            Msg("Fileman Test: El archivo '%s' existe.", name);
        } else {
            Msg("Fileman Test: El archivo '%s' no existe.", name);
        }

        // Probar la obtención del tamaño del archivo
        file = OpenFile(dir, name, "r");
        if (file != NULL) {
            uint size = GetFileSize(file);
            Msg("Fileman Test: Tamaño del archivo: %u bytes.", size);
            CloseFile(file);
        }

        // Probar lectura desde el archivo
        file = OpenFile(dir, name, "r");
        if (file != NULL) {
            char buffer[1024];
            size_t bytes_read = ReadFile(file, buffer, sizeof(char), sizeof(buffer)-1);
            buffer[bytes_read] = '\0';  // Asegurarse de que el buffer se termine correctamente
            Msg("Fileman Test: Leídos %zu bytes: %s", bytes_read, buffer);
            CloseFile(file);
        }

        // Probar la creación de un archivo nuevo (touch)
        if (FileTouch(dir, "new_file.txt")) {
            Msg("Fileman Test: El archivo 'new_file.txt' fue creado o actualizado.");
        } else {
            Msg("Fileman Test: No se pudo crear o actualizar 'new_file.txt'.");
        }

        // Probar el cambio de directorio
        if (ChangeDirectory("..")) {
            Msg("Fileman Test: Cambio de directorio a '..' exitoso.");
        } else {
            Msg("Fileman Test: No se pudo cambiar al directorio padre.");
        }

        // Obtener y mostrar el directorio actual
        char* current_dir = GetCurrentDirectory();
        if (current_dir != NULL) {
            Msg("Fileman Test: Directorio actual: %s", current_dir);
            free(current_dir);
        }

        // Listar archivos del directorio actual
        size_t count;
        str* files = ListCurrentDirectory(&count);
        if (files != NULL) {
            Msg("Fileman Test: Archivos en el directorio actual:");
            for (size_t i = 0; i < count; i++) {
                Msg("Fileman Test:   %s", files[i]);
            }
            FreeFileList(files, count);
        }

        // Probar la navegación a directorios
        if (GoToParentDirectory()) {
            Msg("Fileman Test: Cambiado al directorio padre.");
        }

        if (GoToRootDirectory()) {
            Msg("Fileman Test: Cambiado al directorio raíz.");
        }
    } else {
        Msg("Fileman Test: Modo Debug inactivo, no se realiza la prueba.");
    }
}
