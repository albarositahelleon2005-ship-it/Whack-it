/**
 * \file Puntaje.hpp
 * \brief Cómo se calcula el puntaje de una partida, y la pantalla de
 *        mejores puntajes a la que se llega desde el menú.
 * \date 06/09/2026
 *
 * TODO(equipo): definir aquí cómo se puntúa un golpe (perfecto/bien/fallo),
 * y si el puntaje se guarda en disco para poder mostrar mejores puntajes.
 * Mientras tanto, ActualizarPuntaje/DibujarPuntaje solo dejan navegar hacia
 * y desde la pantalla -el mismo criterio que Instrucciones y Creditos-, para
 * no bloquear el menú por algo que todavía no está diseñado.
 */

#ifndef PUNTAJE_HPP_INCLUDED
#define PUNTAJE_HPP_INCLUDED

#include "Escena.hpp"

/**
 * \brief Procesa la entrada de la pantalla de mejores puntajes.
 * \return Escena_menu si el jugador presionó ESC, o Escena_puntajes
 *         si sigue aquí.
 */
Escena_Estado ActualizarPuntaje();

/**
 * \brief Dibuja la pantalla de mejores puntajes.
 */
void DibujarPuntaje();

#endif // PUNTAJE_HPP_INCLUDED
