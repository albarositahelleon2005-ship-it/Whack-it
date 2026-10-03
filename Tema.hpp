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

// Todo el texto va en el cafe #552000, el mismo de las letras de los botones
// de madera. El fondo de repuesto (para pantallas sin imagen) es crema para
// que ese cafe se lea.
const Color COLOR_FONDO     = { 245, 230, 200, 255 };   ///< Fondo de las pantallas que no traen imagen
const Color COLOR_TEXTO_MADERA = { 85, 32, 0, 255 };     ///< #552000: el café de la tipografía del juego
const Color COLOR_TITULO    = COLOR_TEXTO_MADERA;       ///< Títulos grandes
const Color COLOR_TEXTO     = COLOR_TEXTO_MADERA;       ///< Texto normal
const Color COLOR_TENUE     = {  85,  32,   0, 170 };   ///< Ayudas y notas al pie: el mismo café, más transparente
const Color COLOR_TEXTO_CLARO = { 250, 236, 210, 255 }; ///< Texto sobre fondos oscuros (la madera de la configuración)
const Color COLOR_SELECCION = { 230,  95,  45, 255 };   ///< Lo resaltado: el naranja del letrero WHACK IT!
const Color COLOR_NARANJA   = { 251,  99,  52, 255 };   ///< #FB6334: el naranja de los letreros de los fondos (CREDITOS, PUNTAJE)

// Botones y paneles. El boton "activo" es el de una opcion ya elegida -por
// ejemplo la dificultad seleccionada-, distinto del que solo tiene el raton
// encima. Los tonos son los de la madera de los botones dibujados.
const Color COLOR_BOTON        = { 224, 155,  99, 255 };   ///< Botón en reposo
const Color COLOR_BOTON_HOVER  = { 238, 178, 125, 255 };   ///< Botón bajo el puntero
const Color COLOR_BOTON_ACTIVO = { 188, 106,  51, 255 };   ///< Botón de la opción elegida
const Color COLOR_PANEL        = {  36,  37,  54, 255 };   ///< Fondo de las ventanas virtuales
const Color COLOR_VELO         = {  17,  17,  27, 200 };   ///< Oscurecido detrás de un panel

// Colores de la partida.
const Color COLOR_POZO       = {  24,  24,  37, 255 };   ///< El hueco del pozo
const Color COLOR_TIERRA     = {  69,  71,  90, 255 };   ///< El borde de tierra alrededor
const Color COLOR_VIDA       = { 243, 139, 168, 255 };   ///< Los corazones que quedan
const Color COLOR_VIDA_GASTADA = { 69, 71, 90, 255 };    ///< Los corazones ya perdidos
const Color COLOR_COMBO      = { 249, 226, 175, 255 };   ///< El contador de racha
const Color COLOR_PUNTAJE    = { 166, 227, 161, 255 };   ///< Los puntos acumulados
const Color COLOR_DORADO     = { 250, 200,  60, 255 };   ///< El topo dorado, si falta su gif

#endif // TEMA_HPP_INCLUDED
