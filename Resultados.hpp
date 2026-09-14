/**
 * \file Resultados.hpp
 * \brief La ventana de FIN que aparece cuando se acaban las vidas.
 * \date 13/09/2026
 *
 * **No es una escena**, por la misma razón que la pausa: en el boceto, el
 * cuadro de FIN sale encima de la partida, con el marcador todavía visible
 * detrás. Así que es una ventana virtual que Juego.cpp dibuja encima de su
 * propia pantalla, no una pantalla aparte.
 *
 * Guarda una copia de los números finales con prepararResultados() en lugar de
 * leerlos de la partida: así el panel no depende de que la partida siga viva,
 * y mostrar el resumen es solo dibujar tres datos.
 */

#ifndef RESULTADOS_HPP_INCLUDED
#define RESULTADOS_HPP_INCLUDED

/**
 * \brief Lo que el jugador pidió desde la ventana de FIN.
 */
enum AccionResultados {
    Resultados_ninguna,   ///< Sigue viendo el resumen
    Resultados_menu       ///< Quiere volver al menú principal
};

/**
 * \brief Guarda los números con los que terminó la partida.
 *
 * \param nombre     Nombre del jugador.
 * \param puntaje    Puntos finales.
 * \param mejorCombo La racha más larga que logró.
 */
void prepararResultados(const char* nombre, int puntaje, int mejorCombo);

/**
 * \brief Procesa la entrada de la ventana (una llamada por fotograma).
 * \return La acción elegida, o Resultados_ninguna si sigue viendo el panel.
 */
AccionResultados ActualizarResultados();

/**
 * \brief Dibuja el velo oscuro y el panel de FIN.
 *
 * Se llama **después** de dibujar la partida, para que quede encima.
 */
void DibujarResultados();

#endif // RESULTADOS_HPP_INCLUDED
