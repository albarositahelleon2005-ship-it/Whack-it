/**
 * \file Partida.hpp
 * \brief El estado y las reglas de una partida en curso.
 * \date 13/09/2026
 *
 * Aquí vive **el juego**: el tablero, el puntaje, la racha, las vidas y el
 * reloj que decide cuándo aparece y cuándo se esconde cada objeto. Se separa
 * de Juego.cpp por la misma razón que en Gatorama se separaba Tablero de la
 * pantalla: la pantalla traduce entrada y dibujo, la partida es el modelo.
 *
 * Este módulo **no toca raylib** más que para pedirle el tiempo transcurrido,
 * que además recibe por parámetro. No dibuja, no lee el mouse y no reproduce
 * sonidos: solo dice qué pasó, y quien llama decide qué hacer con eso.
 */

#ifndef PARTIDA_HPP_INCLUDED
#define PARTIDA_HPP_INCLUDED

#include "Tablero.hpp"
#include "ConfigPartida.hpp"

/**
 * \brief Qué pasó cuando el jugador dio un golpe.
 *
 * Se devuelve en vez de reproducir el sonido aquí mismo para que Partida siga
 * sin depender del audio: la pantalla de juego es la que traduce este
 * resultado a un efecto de sonido.
 */
enum ResultadoGolpe {
    Golpe_aire,      ///< Le pegó a un pozo vacío o a ningún pozo: no pasa nada
    Golpe_enemigo,   ///< Le pegó al topo: +1 punto y la racha crece
    Golpe_bomba      ///< Le pegó a la bomba: -2 puntos, -1 vida y adiós racha
};

/**
 * \brief Todo el estado de una partida.
 */
struct Partida {
    Tablero          tablero;      ///< Los pozos y lo que asoma en ellos
    ReglasDificultad reglas;       ///< Copia de las reglas del nivel elegido

    char nombre[NOMBRE_MAX + 1];   ///< Nombre del jugador, para el marcador

    int puntaje;       ///< Puntos acumulados; nunca baja de cero
    int vidas;         ///< Vidas que quedan; al llegar a cero se acaba
    int combo;         ///< Racha actual de topos seguidos sin fallar
    int mejorCombo;    ///< La racha más larga de toda la partida

    float visibleActual;     ///< Cuánto dura visible el próximo objeto; va bajando
    float esperaAparicion;   ///< Cuenta regresiva para la siguiente aparición

    bool terminada;    ///< Verdadero cuando se acabaron las vidas
};

/**
 * \brief Deja la partida lista para jugarse desde cero.
 * \param partida Partida a preparar.
 * \param config  Nombre y dificultad que eligió el jugador.
 */
void iniciarPartida(Partida& partida, const ConfigPartida& config);

/**
 * \brief Avanza el reloj de la partida (una llamada por fotograma).
 *
 * Esconde a los que se les acabó el tiempo, y saca uno nuevo cuando toca. Si
 * la partida ya terminó, no hace nada: así la pantalla puede seguir llamándola
 * sin preguntarse si ya se acabó.
 *
 * \param partida Partida a avanzar.
 * \param dt      Segundos transcurridos desde el fotograma anterior.
 */
void avanzarPartida(Partida& partida, float dt);

/**
 * \brief Aplica un golpe sobre un pozo.
 *
 * \param partida Partida sobre la que se golpea.
 * \param indice  Pozo golpeado, o -1 si el clic no cayó en ninguno.
 * \return Qué resultó del golpe.
 */
ResultadoGolpe golpearPozo(Partida& partida, int indice);

#endif // PARTIDA_HPP_INCLUDED
