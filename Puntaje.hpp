/**
 * \file Puntaje.hpp
 * \brief La pantalla de mejores puntajes, y la tabla que se guarda en disco.
 * \date 06/09/2026
 *
 * Arriba van los tres botones de nivel; debajo, los 10 mejores de ese nivel
 * con su nombre, su racha más larga y sus mapaches atrapados (el puntaje).
 *
 * La tabla en sí (ordenar, leer y escribir el archivo) vive en
 * TablaPuntajes.hpp, que no toca raylib. Aquí solo se guarda la tabla del
 * juego y se dibuja.
 */

#ifndef PUNTAJE_HPP_INCLUDED
#define PUNTAJE_HPP_INCLUDED

#include "Escena.hpp"
#include "Dificultad.hpp"

/**
 * \brief Lee los puntajes guardados. Llamar una sola vez, al arrancar.
 */
void CargarPuntajes();

/**
 * \brief Anota el resultado de una partida y lo guarda en disco de una vez.
 *
 * Se guarda en cuanto termina la partida, y no al cerrar el juego: en la
 * feria es fácil que alguien cierre la ventana de golpe o apague la compu.
 *
 * \param nivel      Nivel en que se jugó.
 * \param nombre     Nombre del jugador.
 * \param puntos     Puntaje final.
 * \param mejorRacha Racha más larga de la partida.
 */
void anotarPuntaje(Dificultad nivel, const char* nombre, int puntos, int mejorRacha);

/**
 * \brief Procesa la entrada de la pantalla de mejores puntajes.
 *
 * Izquierda/derecha o un clic en los botones cambian de nivel.
 *
 * \return Escena_menu si el jugador presionó ESC, o Escena_puntajes
 *         si sigue aquí.
 */
Escena_Estado ActualizarPuntaje();

/**
 * \brief Dibuja la pantalla de mejores puntajes.
 */
void DibujarPuntaje();

#endif // PUNTAJE_HPP_INCLUDED
