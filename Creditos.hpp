/**
 * \file Creditos.hpp
 * \brief Pantalla de créditos del equipo.
 * \date 06/09/2026
 *
 * Pendiente de los nombres reales del equipo. Por ahora solo navega.
 */

#ifndef CREDITOS_HPP_INCLUDED
#define CREDITOS_HPP_INCLUDED

#include "Escena.hpp"

/**
 * \brief Procesa la entrada de la pantalla (una llamada por fotograma).
 * \return Escena_menu si el jugador presionó ESC, o Escena_creditos
 *         si sigue aquí.
 */
Escena_Estado ActualizarCreditos();

/**
 * \brief Dibuja la pantalla de créditos.
 */
void DibujarCreditos();

#endif // CREDITOS_HPP_INCLUDED
