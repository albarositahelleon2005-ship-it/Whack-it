/**
 * \file ConfigPartida.hpp
 * \brief Lo que el jugador elige antes de empezar a jugar.
 * \date 13/09/2026
 *
 * Igual que en Gatorama, vive en su propio archivo y no dentro de la pantalla
 * de configuración: la pantalla **recoge** estos datos y el juego los **usa**.
 * Si el juego tuviera que incluir la pantalla para conocerlos, no se podría
 * arrancar una partida sin pasar por ella.
 */

#ifndef CONFIGPARTIDA_HPP_INCLUDED
#define CONFIGPARTIDA_HPP_INCLUDED

#include "Dificultad.hpp"

/// Cuántas letras puede tener el nombre del jugador.
const int NOMBRE_MAX = 12;

/**
 * \brief Todo lo que hay que saber para arrancar una partida.
 */
struct ConfigPartida {
    Dificultad dificultad;              ///< Nivel elegido en la pantalla de configuración
    char       nombre[NOMBRE_MAX + 1];  ///< Nombre del jugador (+1 por el '\0' final)
};

/**
 * \brief La configuración con la que arranca el juego.
 * \return Configuración lista para usarse: dificultad fácil y nombre "Player 1".
 */
ConfigPartida configPorDefecto();

#endif // CONFIGPARTIDA_HPP_INCLUDED
