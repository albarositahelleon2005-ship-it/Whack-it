/**
 * \file ConfigPartida.cpp
 * \brief Implementación de la configuración de partida.
 * \date 13/09/2026
 */

#include <cstring>

#include "ConfigPartida.hpp"

ConfigPartida configPorDefecto()
{
    ConfigPartida config;

    config.dificultad = Dificultad_facil;

    // Con un nombre ya puesto se puede jugar de inmediato; quien quiera el suyo
    // lo borra y lo escribe.
    strcpy(config.nombre, "Player 1");

    return config;
}
