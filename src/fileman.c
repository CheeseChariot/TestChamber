#include "fileman.h"

FM OpenFile(str Dir, str Name, str Mode) {
    FM File = (FM) malloc(sizeof(struct FM));
    if (File == NULL) return NULL;

    char FilePath[256];
    snprintf(FilePath, sizeof(FilePath), "%s/%s", Dir, Name);

    File->FP = fopen(FilePath, Mode);

    if (File->FP == NULL) {
        free(File);
        return NULL;
    }

    snprintf(File->Mode, sizeof(File->Mode), "%s", Mode);

    return File;
}