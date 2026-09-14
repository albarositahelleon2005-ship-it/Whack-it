/**
 * \file Menu.hpp
 * \brief Menú principal del juego.
 * \date 06/09/2026
 */

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include "Escena.hpp"

/**
 * \brief Procesa la entrada del menú principal (una llamada por fotograma).
 *
 * Mueve la opción resaltada con las flechas y confirma con Enter. Cuál opción
 * está resaltada es asunto interno de Menu.cpp: quien llama no necesita saberlo.
 *
 * \return La escena a la que hay que cambiar, o Escena_menu si el usuario sigue
 *         navegando sin haber elegido nada.
 */
Escena_Estado ActualizarMenu();

/**
 * \brief Dibuja el menú principal (una llamada por fotograma).
 *
 * Solo dibuja: no lee entrada ni cambia nada.
 */
void DibujarMenu();

#endif // MENU_HPP_INCLUDED
