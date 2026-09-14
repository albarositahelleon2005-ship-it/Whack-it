/**
 * \file BarraSuperior.hpp
 * \brief Los dos íconos de las esquinas de arriba, en todas las pantallas.
 * \date 13/09/2026
 *
 * A la derecha siempre está el engrane de ajustes. A la izquierda cambia según
 * dónde estemos: la flecha de regresar en las pantallas normales, el botón de
 * pausa mientras se juega, y nada en el menú principal (no hay a dónde volver).
 *
 * Se dibuja **una sola vez por fotograma desde main.cpp**, encima de lo que
 * haya dibujado la pantalla actual. Así ninguna pantalla tiene que preocuparse
 * por estos íconos ni repetir su código.
 */

#ifndef BARRASUPERIOR_HPP_INCLUDED
#define BARRASUPERIOR_HPP_INCLUDED

/**
 * \brief Qué ícono va en la esquina superior izquierda.
 */
enum IconoIzquierdo {
    Izq_ninguno,    ///< No se dibuja nada (menú principal)
    Izq_regresar,   ///< Flecha para volver al menú
    Izq_pausa       ///< Botón de pausa (durante la partida)
};

/**
 * \brief Dibuja la barra superior.
 * \param izquierdo Qué ícono va del lado izquierdo.
 */
void dibujarBarraSuperior(IconoIzquierdo izquierdo);

/**
 * \brief Si dieron clic en el ícono de la izquierda este fotograma.
 *
 * Qué significa ese clic lo decide quien llama, según en qué pantalla esté:
 * regresar al menú o pausar la partida.
 */
bool iconoIzquierdoClicado();

/**
 * \brief Si dieron clic en el engrane de ajustes este fotograma.
 */
bool iconoAjustesClicado();

#endif // BARRASUPERIOR_HPP_INCLUDED
