/**
 * \file Dificultad.hpp
 * \brief Los niveles de dificultad y las reglas que cada uno impone.
 * \date 13/09/2026
 *
 * En Whack it la dificultad no es solo una etiqueta: decide cuántos pozos hay,
 * cuántas vidas tiene el jugador, cuánto tiempo se queda visible un objeto y
 * cada cuándo aparece uno nuevo. Todo eso vive en ReglasDificultad, en un solo
 * lugar, para que ajustar el balance del juego sea editar este archivo y no
 * andar cazando números sueltos por Partida.cpp.
 */

#ifndef DIFICULTAD_HPP_INCLUDED
#define DIFICULTAD_HPP_INCLUDED

const int NUM_DIFICULTADES = 3;

/// Cuántos pozos puede haber como máximo (los del nivel difícil).
const int MAX_POZOS = 10;

/// En cuántas filas se acomodan los pozos, como máximo.
const int MAX_FILAS = 3;

//***********************************************
// REGLAS QUE NO CAMBIAN CON EL NIVEL
//***********************************************

/// Cuánto se queda en el pozo la imagen del topo aplastado o de la explosión.
const float DURACION_APLASTADO = 0.5f;

/// Cuántos puntos da el topo dorado (el normal da 1). Estuvo en 3, pero el
/// equipo prefirió que todo mapache atrapado valga 1, sea dorado o no. Se deja
/// la constante por si se quiere volver a premiar al dorado.
const int PUNTOS_PREMIUM = 1;

// Probabilidad del topo dorado. No es fija: sube mientras más tiempo lleva sin
// salir, para que ninguna partida se quede sin verlo. Se cuenta en objetos
// (topos y bombas) desde el último dorado, y al salir uno la cuenta vuelve a 0.
const int PORCENTAJE_PREMIUM        = 15;   ///< Probabilidad normal
const int PORCENTAJE_PREMIUM_TARDE  = 50;   ///< Pasados PREMIUM_TARDE_DESPUES objetos sin dorado
const int PREMIUM_TARDE_DESPUES     = 20;
const int PREMIUM_SEGURO_EN         = 30;   ///< El objeto número 30 sin dorado es dorado sí o sí

/// Cada cuántos topos escapados (en toda la partida, no seguidos) se pierde una vida.
const int ESCAPES_POR_VIDA = 3;

/**
 * \brief Nivel de dificultad de la partida.
 *
 * El valor del enum es también el índice dentro de NOMBRES_DIFICULTAD, así
 * que NOMBRES_DIFICULTAD[Dificultad_facil] da el nombre del nivel fácil.
 */
enum Dificultad {
    Dificultad_facil,
    Dificultad_normal,
    Dificultad_dificil
};

/**
 * \brief Todo lo que un nivel de dificultad decide sobre una partida.
 *
 * Se piden una sola vez con reglasDe() al iniciar la partida y se guardan
 * dentro de ella: así la partida en curso no depende de que nadie vuelva a
 * consultar la tabla mientras se juega.
 */
struct ReglasDificultad {
    int   pozos;                ///< Cuántos pozos se dibujan
    int   vidas;                ///< Con cuántas vidas empieza el jugador
    float visibleInicial;       ///< Segundos que dura visible el primer objeto
    float visibleMinimo;        ///< Piso de ese tiempo; nunca baja de aquí
    float pasoReduccion;        ///< Cuánto se recorta ese tiempo cada \p aparicionesPorPaso
    int   aparicionesPorPaso;   ///< Cada cuántas apariciones se recorta el tiempo visible
    int   maxSimultaneos;       ///< Cuántos objetos puede haber en pantalla a la vez (aplastados incluidos)
    int   porcentajeBomba;      ///< De cada 100 apariciones, cuántas son bomba
    int   filas;                ///< En cuántas filas se acomodan los pozos
    const int* pozosPorFila;    ///< Cuántos pozos lleva cada fila (\p filas elementos)
};

// Los nombres van sin acento a proposito: la fuente que trae raylib de
// fabrica solo cubre ASCII.
extern const char* NOMBRES_DIFICULTAD[NUM_DIFICULTADES];

/**
 * \brief El nombre que hay que mostrarle al jugador para un nivel.
 * \param nivel Nivel de dificultad.
 * \return Cadena con el nombre a dibujar.
 */
const char* nombreDeDificultad(Dificultad nivel);

/**
 * \brief Las reglas completas de un nivel.
 * \param nivel Nivel de dificultad.
 * \return La tabla de reglas correspondiente.
 */
ReglasDificultad reglasDe(Dificultad nivel);

#endif // DIFICULTAD_HPP_INCLUDED
