/**
 * \file Ajustes.hpp
 * \brief La ventana de ajustes de volumen que abre el engrane.
 * \date 13/09/2026
 *
 * **No es una escena**, es una ventana virtual: igual que la pausa, se dibuja
 * encima de lo que haya debajo, que se sigue viendo oscurecido. Por eso no
 * aparece en el enum de Escena.hpp.
 *
 * A diferencia de la pausa, esta ventana se puede abrir desde **cualquier**
 * pantalla, así que quien la maneja es main.cpp: mientras está abierta, la
 * pantalla de abajo no recibe entrada ni avanza su reloj.
 */

#ifndef AJUSTES_HPP_INCLUDED
#define AJUSTES_HPP_INCLUDED

/**
 * \brief Abre la ventana de ajustes.
 */
void abrirAjustes();

/**
 * \brief Si la ventana de ajustes está abierta en este momento.
 */
bool ajustesAbiertos();

/**
 * \brief Procesa la entrada de la ventana (una llamada por fotograma).
 *
 * Aplica los cambios de volumen y se cierra sola con el botón Cerrar o con ESC.
 * Con el teclado: arriba/abajo eligen fila, izquierda/derecha bajan y suben
 * el volumen, y Enter sobre Cerrar la cierra.
 */
void ActualizarAjustes();

/**
 * \brief Dibuja el velo oscuro y el panel de ajustes.
 *
 * Se llama **al final** del dibujado, para que quede encima de todo.
 */
void DibujarAjustes();

#endif // AJUSTES_HPP_INCLUDED
