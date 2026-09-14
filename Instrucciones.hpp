/**
 * \file Instrucciones.hpp
 * \brief Pantalla que explica cómo se juega.
 * \date 06/09/2026
 *
 * Todavía no existe el texto de las reglas (depende de cómo quede la
 * mecánica de notas/carriles), así que por ahora solo navega: se puede
 * entrar y salir con ESC. Cuando se definan las reglas, DibujarInstrucciones
 * deja de llamar a dibujarPantallaPendiente y dibuja el contenido real.
 */

#ifndef INSTRUCCIONES_HPP_INCLUDED
#define INSTRUCCIONES_HPP_INCLUDED

#include "Escena.hpp"

/**
 * \brief Procesa la entrada de la pantalla (una llamada por fotograma).
 * \return Escena_menu si el jugador presionó ESC, o Escena_instrucciones
 *         si sigue aquí.
 */
Escena_Estado ActualizarInstrucciones();

/**
 * \brief Dibuja la pantalla de instrucciones.
 */
void DibujarInstrucciones();

#endif // INSTRUCCIONES_HPP_INCLUDED
