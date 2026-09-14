/**
 * \file Juego.hpp
 * \brief La pantalla de la partida: traduce entrada y dibujo.
 * \date 13/09/2026
 *
 * Esta pantalla **no tiene reglas propias**. Las reglas viven en Partida.hpp y
 * el acomodo en pantalla en VistaTablero.hpp; aquí solo se leen el mouse y el
 * teclado, se le pasan a la partida, y se dibuja lo que quedó. Las dos
 * ventanas virtuales -pausa y FIN- también se manejan desde aquí, porque
 * ambas van encima de esta misma pantalla.
 */

#ifndef JUEGO_HPP_INCLUDED
#define JUEGO_HPP_INCLUDED

#include "Escena.hpp"
#include "ConfigPartida.hpp"

/**
 * \brief Arma una partida nueva con la configuración dada.
 * \param config Nombre y dificultad elegidos en la pantalla anterior.
 */
void IniciarPartida(const ConfigPartida& config);

/**
 * \brief Abre la ventana de pausa.
 *
 * La usa main.cpp cuando le dan clic al botón de pausa de la barra superior.
 * Si la partida ya terminó no hace nada: en el resumen no hay qué pausar.
 */
void AbrirPausa();

/**
 * \brief Procesa la entrada de la partida (una llamada por fotograma).
 * \return La escena a la que hay que cambiar, o Escena_juego si se sigue
 *         jugando.
 */
Escena_Estado ActualizarJuego();

/**
 * \brief Dibuja la partida y, si toca, la pausa o el FIN encima.
 */
void DibujarJuego();

#endif // JUEGO_HPP_INCLUDED
