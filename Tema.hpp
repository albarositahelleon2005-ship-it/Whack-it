/**
 * \file Tema.hpp
 * \brief Paleta de colores del juego, declarada en un solo lugar.
 * \date 13/09/2026
 *
 * Mismo criterio que en Gatorama: cambiar el aspecto del juego debe ser
 * editar este archivo, no salir a cazar literales por todos lados.
 */

#ifndef TEMA_HPP_INCLUDED
#define TEMA_HPP_INCLUDED

#include "raylib.h"

const Color COLOR_FONDO     = {  30,  30,  46, 255 };   ///< Fondo de todas las pantallas
const Color COLOR_TITULO    = { 203, 166, 247, 255 };   ///< Títulos grandes
const Color COLOR_TEXTO     = { 166, 173, 200, 255 };   ///< Texto normal
const Color COLOR_SELECCION = { 137, 180, 250, 255 };   ///< Opción resaltada del menú
const Color COLOR_TENUE     = { 108, 112, 134, 255 };   ///< Ayudas y notas al pie

// Botones y paneles. El boton "activo" es el de una opcion ya elegida -por
// ejemplo la dificultad seleccionada-, distinto del que solo tiene el raton
// encima.
const Color COLOR_BOTON        = {  49,  50,  68, 255 };   ///< Botón en reposo
const Color COLOR_BOTON_HOVER  = {  69,  71,  90, 255 };   ///< Botón bajo el puntero
const Color COLOR_BOTON_ACTIVO = { 137, 180, 250, 255 };   ///< Botón de la opción elegida
const Color COLOR_PANEL        = {  36,  37,  54, 255 };   ///< Fondo de las ventanas virtuales
const Color COLOR_VELO         = {  17,  17,  27, 200 };   ///< Oscurecido detrás de un panel

// Colores de la partida.
const Color COLOR_POZO       = {  24,  24,  37, 255 };   ///< El hueco del pozo
const Color COLOR_TIERRA     = {  69,  71,  90, 255 };   ///< El borde de tierra alrededor
const Color COLOR_VIDA       = { 243, 139, 168, 255 };   ///< Los corazones que quedan
const Color COLOR_VIDA_GASTADA = { 69, 71, 90, 255 };    ///< Los corazones ya perdidos
const Color COLOR_COMBO      = { 249, 226, 175, 255 };   ///< El contador de racha
const Color COLOR_PUNTAJE    = { 166, 227, 161, 255 };   ///< Los puntos acumulados

#endif // TEMA_HPP_INCLUDED
