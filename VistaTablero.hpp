/**
 * \file VistaTablero.hpp
 * \brief Cómo se ve el tablero en pantalla, y dónde cae cada clic.
 * \date 13/09/2026
 *
 * Este módulo es el traductor entre el modelo y los píxeles, igual que en
 * Gatorama: Tablero.hpp dice "el pozo 3 tiene un topo", y aquí se decide que
 * el pozo 3 va en tal lugar de la pantalla.
 *
 * Que la geometría viva en un solo lugar es lo que hace que el dibujo y la
 * detección del clic **no puedan desalinearse**: las dos preguntan aquí.
 */

#ifndef VISTATABLERO_HPP_INCLUDED
#define VISTATABLERO_HPP_INCLUDED

#include "raylib.h"

#include "Partida.hpp"

/**
 * \brief El rectángulo del hoyo de un pozo.
 * \param reglas Reglas del nivel (de ahí salen cuántos pozos hay y su acomodo).
 * \param indice Pozo, desde cero.
 */
Rectangle areaPozo(const ReglasDificultad& reglas, int indice);

/**
 * \brief El rectángulo donde se dibuja lo que asoma de un pozo.
 *
 * Va **encima** del hoyo, no dentro: es el muñeco que se asoma.
 *
 * \param reglas Reglas del nivel.
 * \param indice Pozo, desde cero.
 */
Rectangle areaObjeto(const ReglasDificultad& reglas, int indice);

/**
 * \brief Qué pozo le corresponde a un punto de la pantalla.
 *
 * Cuenta tanto el muñeco como el hoyo, para que el jugador no tenga que
 * apuntarle exacto a la imagen. Solo considera los pozos que tienen algo que
 * golpear: con 3 filas la cabeza de un muñeco tapa un poco el hoyo de arriba,
 * y si ese hoyo vacío ganara el clic, el golpe se perdería y el topo contaría
 * como escapado aunque el jugador sí le atinó.
 *
 * \param partida Partida en curso (sus reglas y qué hay en cada pozo).
 * \param punto   Posición del clic.
 * \return El índice del pozo, o -1 si el clic no cayó sobre nada golpeable.
 */
int pozoEn(const Partida& partida, Vector2 punto);

/**
 * \brief Dibuja todos los pozos y lo que asoma en ellos.
 * \param partida Partida en curso.
 */
void dibujarTablero(const Partida& partida);

/**
 * \brief Dibuja el marcador: nombre, puntos, racha y vidas.
 * \param partida Partida en curso.
 */
void dibujarMarcador(const Partida& partida);

#endif // VISTATABLERO_HPP_INCLUDED
