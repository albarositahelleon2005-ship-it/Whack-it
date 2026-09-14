/**
 * \file Escena.hpp
 * \brief Las pantallas del juego, en un solo lugar.
 * \date 13/09/2026
 *
 * Igual que en Gatorama: el bucle principal guarda uno de estos valores y
 * decide con él qué actualizar y qué dibujar en cada vuelta. No hay ventanas
 * de verdad, solo esta variable.
 */

#ifndef ESCENA_HPP_INCLUDED
#define ESCENA_HPP_INCLUDED

/**
 * \brief Pantallas del juego.
 *
 * Tres cosas que **no están en esta lista** a propósito, porque no son
 * pantallas sino ventanas que se dibujan **encima** de la que esté abajo, que
 * se sigue viendo: la **pausa** y el **FIN de partida** (ambas encima del
 * juego, en Juego.cpp) y los **ajustes** de volumen (encima de cualquier
 * pantalla, en main.cpp).
 */
enum Escena_Estado {
    Escena_menu,            ///< Menú principal
    Escena_configuracion,   ///< Poner el nombre y elegir la dificultad
    Escena_puntajes,        ///< Mejores puntajes
    Escena_instrucciones,   ///< Cómo se juega
    Escena_opciones,        ///< Pantalla de ajustes - sin acceso desde el menu por ahora
    Escena_creditos,        ///< Quién hizo el juego
    Escena_juego,           ///< La partida en curso
    Escena_salir            ///< No dibuja nada; le avisa al bucle que termine
};

#endif // ESCENA_HPP_INCLUDED
