/**
 * \file Aleatorio.cpp
 * \brief Implementaci&oacute;n de la generaci&oacute;n de n&uacute;meros aleatorios.
 */

#include <cstdlib>

int aleatorio(int inicio, int fin)
{
    return inicio + rand() % (fin-inicio+1);
}
