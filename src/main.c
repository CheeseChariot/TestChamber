#include "fileman.h"

int main() {
    Msg("Mensaje de inicio.");

    Debug("Modo Depurador activado.");
    if(!DebugStatus())Msg("Modo Depurador apagado.");

    FilemanTest();

    Msg("Cierre del programa.");

    End
}