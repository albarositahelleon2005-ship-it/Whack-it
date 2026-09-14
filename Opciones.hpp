/**
 * \file Opciones.hpp
 * \brief Pantalla de ajustes del juego (audio, controles, etc.).
 * \date 06/09/2026
 *
 * Pendiente de contenido real hasta que se decida qué ajustes tiene sentido
 * exponer (volumen de música/efectos, teclas, etc.). Por ahora solo navega.
 */

#ifndef OPCIONES_HPP_INCLUDED
#define OPCIONES_HPP_INCLUDED

#include "Escena.hpp"

/**
 * \brief Procesa la entrada de la pantalla (una llamada por fotograma).
 * \return Escena_menu si el jugador presionó ESC, o Escena_opciones
 *         si sigue aquí.
 */
Escena_Estado ActualizarOpciones();

/**
 * \brief Dibuja la pantalla de opciones.
 */
void DibujarOpciones();

#endif // OPCIONES_HPP_INCLUDED
