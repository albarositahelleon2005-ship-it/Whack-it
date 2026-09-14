/**
 * \file Tablero.hpp
 * \brief El área de juego de Whack it: la hilera de pozos y qué hay en cada uno.
 * \date 13/09/2026
 *
 * Esto es **solo el modelo**: dice qué pozos hay y qué asoma en cada uno, pero
 * no sabe en qué píxel se dibuja ninguno (eso es VistaTablero.hpp) ni cuántos
 * puntos vale golpearlo (eso es Partida.hpp). Separarlo así es lo que permite
 * cambiar el acomodo en pantalla sin tocar una sola regla del juego.
 */

#ifndef TABLERO_HPP_INCLUDED
#define TABLERO_HPP_INCLUDED

#include "Dificultad.hpp"

/**
 * \brief Qué está asomado en un pozo.
 */
enum ContenidoPozo {
    Pozo_vacio,     ///< No hay nada; golpearlo no hace nada
    Pozo_enemigo,   ///< El topo: golpearlo suma
    Pozo_bomba      ///< La bomba: golpearla cuesta caro
};

/**
 * \brief Un pozo del tablero.
 */
struct Pozo {
    ContenidoPozo contenido;   ///< Qué asoma en este momento
    float restante;            ///< Segundos que le quedan antes de esconderse
    float vivo;                ///< Segundos que lleva asomado (para animar el gif)
};

/**
 * \brief El área de juego completa.
 */
struct Tablero {
    Pozo pozos[MAX_POZOS];   ///< Siempre se reserva el máximo; \p cantidad dice cuántos se usan
    int  cantidad;           ///< Cuántos pozos tiene esta partida (según la dificultad)
};

/**
 * \brief Deja el tablero con todos los pozos vacíos.
 * \param tablero  Tablero a limpiar.
 * \param cantidad Cuántos pozos va a tener (1..MAX_POZOS).
 */
void vaciarTablero(Tablero& tablero, int cantidad);

/**
 * \brief Cuántos pozos tienen algo asomado.
 * \param tablero Tablero a revisar.
 * \return El número de pozos ocupados.
 */
int pozosOcupados(const Tablero& tablero);

/**
 * \brief Elige al azar un pozo que esté vacío.
 *
 * Se recorren los pozos libres y se escoge uno entre ellos, en vez de tirar un
 * índice al azar y reintentar si cayó ocupado: así nunca se cicla, ni siquiera
 * cuando casi todos están llenos.
 *
 * \param tablero Tablero a revisar.
 * \return El índice del pozo elegido, o -1 si no hay ninguno libre.
 */
int pozoLibreAlAzar(const Tablero& tablero);

#endif // TABLERO_HPP_INCLUDED
