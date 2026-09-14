/**
 * \file Animacion.hpp
 * \brief Convierte un GIF animado en algo que el juego pueda dibujar.
 * \date 13/09/2026
 *
 * raylib sabe leer un GIF con todos sus cuadros (LoadImageAnim), pero los deja
 * en memoria RAM, uno tras otro y a su tamaño original. Dibujarlos así sería
 * carísimo: bomba.gif mide 638x550 y trae 99 cuadros, o sea más de 130 MB.
 *
 * Lo que hace este módulo es el paso intermedio de siempre en videojuegos:
 * al arrancar, achica cada cuadro al tamaño en que de verdad se va a ver y los
 * acomoda en una sola imagen tipo cuadrícula -una **hoja de sprites**-, que se
 * sube a la tarjeta de video una sola vez. A partir de ahí, animar es recortar
 * el pedacito que toca según el reloj: ni un solo byte se vuelve a mover.
 *
 * Los cuadros se achican respetando la proporción original y se centran, para
 * que un gif que no sea cuadrado no salga aplastado.
 */

#ifndef ANIMACION_HPP_INCLUDED
#define ANIMACION_HPP_INCLUDED

#include "raylib.h"

/**
 * \brief Un GIF ya listo para dibujarse.
 */
struct Animacion {
    Texture2D hoja;            ///< Todos los cuadros en una sola textura
    int   cuadros;             ///< Cuántos cuadros trae (0 si no se pudo cargar)
    int   columnas;            ///< Cuántos cuadros por fila en la hoja
    int   lado;                ///< Tamaño en píxeles de cada cuadro (son cuadrados)
    float duracionCuadro;      ///< Segundos que dura cada cuadro
};

/**
 * \brief Carga un GIF animado y arma su hoja de sprites.
 *
 * Llamar **después** de InitWindow: subir la textura necesita que ya exista el
 * contexto de video.
 *
 * \param ruta             Archivo .gif a cargar.
 * \param lado             Tamaño al que se achica cada cuadro, en píxeles.
 * \param cuadrosPorSegundo A qué ritmo se reproduce.
 * \return La animación lista, o una con \p cuadros en 0 si el archivo no se
 *         pudo leer (el juego sigue corriendo; solo se dibuja un relleno).
 */
Animacion cargarAnimacion(const char* ruta, int lado, float cuadrosPorSegundo);

/**
 * \brief Libera la textura de una animación.
 * \param animacion Animación a liberar; queda marcada como vacía.
 */
void descargarAnimacion(Animacion& animacion);

/**
 * \brief Si la animación se cargó bien y se puede dibujar.
 * \param animacion Animación a revisar.
 */
bool animacionLista(const Animacion& animacion);

/**
 * \brief Dibuja el cuadro que le toca a esta animación en este momento.
 *
 * El cuadro sale del tiempo, no de un contador guardado: así dos pozos que
 * llevan distinto rato asomados muestran distinto cuadro sin tener que
 * guardar nada por separado.
 *
 * \param animacion Animación ya cargada.
 * \param destino   Rectángulo de pantalla donde se pinta (se estira a él).
 * \param tiempo    Segundos que lleva corriendo la animación.
 */
void dibujarAnimacion(const Animacion& animacion, Rectangle destino, float tiempo);

#endif // ANIMACION_HPP_INCLUDED
