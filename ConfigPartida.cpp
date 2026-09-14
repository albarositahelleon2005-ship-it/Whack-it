/**
 * \file ConfigPartida.cpp
 * \brief Implementación de la configuración de partida.
 * \date 13/09/2026
 */

#include "ConfigPartida.hpp"

ConfigPartida configPorDefecto()
{
    ConfigPartida config;

    config.dificultad = Dificultad_facil;

    // Cadena vacia: el primer caracter ya es el fin de cadena. La pantalla de
    // configuracion no deja iniciar hasta que haya al menos una letra.
    config.nombre[0] = '\0';

    return config;
}
