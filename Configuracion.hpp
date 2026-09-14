/**
 * \file Configuracion.hpp
 * \brief Pantalla donde se pone el nombre y se elige la dificultad.
 * \date 13/09/2026
 *
 * Esta pantalla **no arma la partida**: solo llena un ConfigPartida y avisa
 * que ya quedó. Quien arma la partida de verdad es la pantalla de juego.
 * Separarlo permite iniciar una partida sin pasar por aquí, que es justo lo
 * que hace falta para probarla.
 */

#ifndef CONFIGURACION_HPP_INCLUDED
#define CONFIGURACION_HPP_INCLUDED

#include "Escena.hpp"
#include "ConfigPartida.hpp"

/**
 * \brief Procesa la entrada de la pantalla (una llamada por fotograma).
 *
 * \param config Configuración que se va llenando conforme el jugador escribe y elige.
 * \return Escena_juego si le dio a Iniciar, Escena_menu si canceló con ESC, o
 *         Escena_configuracion si sigue eligiendo.
 */
Escena_Estado ActualizarConfiguracion(ConfigPartida& config);

/**
 * \brief Dibuja la pantalla con las opciones y el resumen.
 * \param config Configuración actual, para marcar lo que ya está elegido.
 */
void DibujarConfiguracion(const ConfigPartida& config);

#endif // CONFIGURACION_HPP_INCLUDED
