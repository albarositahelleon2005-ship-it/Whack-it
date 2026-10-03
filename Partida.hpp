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
    Golpe_aire,      ///< Le pegó a un pozo vacío, ya golpeado o a ningún pozo: no pasa nada
    Golpe_enemigo,   ///< Le pegó al topo: +1 punto y la racha crece
    Golpe_premium,   ///< Le pegó al topo dorado: +PUNTOS_PREMIUM y la racha crece
    Golpe_bomba      ///< Le pegó a la bomba: -2 puntos, -1 vida y adiós racha
};

/**
 * \brief Cuánto se queda el "3/3" en el marcador al perder una vida por escapes.
 *
 * Sin esto, el tercer escape pasaba de "2/3" directo a "0/3" en el mismo
 * fotograma en que se rompía el corazón, y parecía que el contador se
 * reiniciaba sin castigo y que la vida se perdía hasta el cuarto.
 */
const float DURACION_AVISO_ESCAPES = 1.0f;

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
    int   apariciones;       ///< Objetos que han salido en toda la partida
    int   sinPremium;        ///< Objetos que han salido desde el último topo dorado
    int   escapados;         ///< Topos (normales o dorados) que se escondieron sin ser golpeados
    float avisoEscapes;      ///< Segundos que el marcador sigue mostrando "3/3" tras perder una vida por escapes

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

/**
 * \brief Probabilidad (de 0 a 100) de que el siguiente objeto sea topo dorado.
 * \param sinPremium Objetos que han salido desde el último dorado.
 */
int porcentajePremium(int sinPremium);

#endif // PARTIDA_HPP_INCLUDED
