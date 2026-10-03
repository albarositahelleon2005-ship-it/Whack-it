/**
 * \file Pausa.hpp
 * \brief La ventana de pausa que se dibuja encima de la pantalla de juego.
 * \date 06/09/2026
 *
 * **No es una escena.** Es una ventana virtual: la pantalla de juego se sigue
 * dibujando debajo, oscurecida, y este panel va encima. Por eso la pausa no
 * aparece en el enum de Escena.hpp, sino como una bandera dentro de Juego.cpp.
 *
 * Este m&oacute;dulo solo dibuja el panel y reporta qu&eacute; bot&oacute;n se apret&oacute;. **No** detiene
 * nada por su cuenta: quien decide congelar el reloj es la pantalla de juego.
 */

#ifndef PAUSA_HPP_INCLUDED
#define PAUSA_HPP_INCLUDED

/**
 * \brief Lo que el jugador pidi&oacute; desde la ventana de pausa.
 */
enum AccionPausa {
    Pausa_ninguna,      ///< Sigue abierta, nadie ha elegido
    Pausa_continuar,    ///< Cerrar y seguir jugando
    Pausa_reiniciar,    ///< Volver a repartir esta misma partida
    Pausa_menu          ///< Abandonar y regresar al men&uacute; principal
};

/**
 * \brief Deja resaltado "Continuar". Llamar cada vez que se abre la pausa.
 *
 * Sin esto la pausa se abr&iacute;a con el &uacute;ltimo bot&oacute;n que se us&oacute;, y un Enter
 * r&aacute;pido pod&iacute;a mandar al men&uacute; en vez de seguir jugando.
 */
void prepararPausa();

/**
 * \brief Procesa la entrada de la ventana de pausa (una llamada por fotograma).
 *
 * Las flechas arriba/abajo mueven el resaltado y Enter elige ese bot&oacute;n; el
 * rat&oacute;n tambi&eacute;n sirve. ESC hace lo mismo que el bot&oacute;n Continuar: la tecla
 * que abre es la que cierra.
 *
 * \return La acci&oacute;n elegida, o Pausa_ninguna si el jugador sigue viendo el panel.
 */
AccionPausa ActualizarPausa();

/**
 * \brief Dibuja el velo oscuro y el panel de pausa.
 *
 * Se llama **despu&eacute;s** de dibujar el tablero, para que quede encima.
 */
void DibujarPausa();

#endif // PAUSA_HPP_INCLUDED
