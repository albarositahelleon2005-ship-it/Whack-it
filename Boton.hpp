/**
 * \file Boton.hpp
 * \brief Botones con rat&oacute;n, compartidos por todas las pantallas.
 * \date 06/09/2026
 *
 * Un bot&oacute;n aqu&iacute; no es un objeto que se guarde en ning&uacute;n lado: es un rect&aacute;ngulo
 * al que se le pregunta si lo clicaron y al que se le pide que se dibuje. Ese es
 * el estilo de raylib, y significa que agregar un bot&oacute;n a una pantalla es agregar
 * dos llamadas, sin registrar nada ni acordarse de liberarlo.
 */

#ifndef BOTON_HPP_INCLUDED
#define BOTON_HPP_INCLUDED

#include "raylib.h"

/**
 * \brief Si el puntero est&aacute; encima de un rect&aacute;ngulo.
 * \param rec Zona a revisar.
 * \return Verdadero si el rat&oacute;n cae dentro.
 */
bool ratonEncima(Rectangle rec);

/**
 * \brief Si acaban de hacer clic sobre un rect&aacute;ngulo.
 *
 * Va en la parte de **actualizar**, no en la de dibujar: cambia lo que va a pasar.
 *
 * \param rec Zona del bot&oacute;n.
 * \return Verdadero solo en el fotograma en que se presion&oacute; el bot&oacute;n izquierdo
 *         estando encima.
 */
bool botonClicado(Rectangle rec);

/**
 * \brief Dibuja un bot&oacute;n con su etiqueta centrada.
 *
 * \param rec          D&oacute;nde va.
 * \param etiqueta     Texto del bot&oacute;n.
 * \param seleccionado Verdadero si representa una opci&oacute;n ya elegida (por ejemplo la
 *                     dificultad actual). Es distinto de tener el rat&oacute;n encima: eso
 *                     se detecta solo.
 */
void dibujarBoton(Rectangle rec, const char* etiqueta, bool seleccionado);

/**
 * \brief Arma un rect&aacute;ngulo, para no repetir la construcci&oacute;n a cada rato.
 * \param x      Esquina izquierda.
 * \param y      Esquina superior.
 * \param ancho  Ancho.
 * \param alto   Alto.
 * \return El rect&aacute;ngulo.
 */
Rectangle rectangulo(float x, float y, float ancho, float alto);

/**
 * \brief Dibuja un bot&oacute;n de imagen (un &iacute;cono sin texto).
 *
 * Se resalta agrand&aacute;ndose un poco (CRECE_ICONO, en Iconos.hpp) cuando el rat&oacute;n
 * est&aacute; encima, o cuando \p resaltado es verdadero (por ejemplo, porque el
 * teclado lo se&ntilde;ala). La textura se estira exactamente al tama&ntilde;o de \p rec, as&iacute;
 * que conviene que \p rec sea cuadrado si la imagen lo es.
 *
 * \param rec       D&oacute;nde va el bot&oacute;n.
 * \param textura   Imagen ya cargada (ver Iconos.hpp).
 * \param resaltado Verdadero si hay que marcarlo aunque el rat&oacute;n no est&eacute; encima.
 */
void dibujarBotonIcono(Rectangle rec, Texture2D textura, bool resaltado);

/**
 * \brief Dibuja un bot&oacute;n de madera ya dibujado (una imagen con su texto).
 *
 * La imagen ya trae su versi&oacute;n apagada o prendida, as&iacute; que aqu&iacute; no se
 * resalta nada: quien llama elige cu&aacute;l textura pasar. Si la imagen no carg&oacute;
 * (id 0), se dibuja un bot&oacute;n de texto normal con \p repuesto, para que el
 * juego se pueda seguir usando.
 *
 * \param rec      D&oacute;nde va; la imagen se estira a este tama&ntilde;o.
 * \param textura  Imagen del bot&oacute;n (ver Iconos.hpp).
 * \param repuesto Texto del bot&oacute;n si falta la imagen.
 * \param prendido Si va resaltado; solo afecta al bot&oacute;n de repuesto.
 */
void dibujarBotonImagen(Rectangle rec, Texture2D textura, const char* repuesto, bool prendido);

/**
 * \brief Dibuja el bot&oacute;n de madera de un nivel de dificultad.
 *
 * Lo usan la configuraci&oacute;n y la tabla de puntajes. Si falta la imagen
 * apagada se usa la prendida oscurecida, y si faltan las dos, un bot&oacute;n de
 * texto.
 *
 * \param area    D&oacute;nde va; debe medir ANCHO_BOTON_NIVEL x ALTO_BOTON_NIVEL.
 * \param nivel   0 f&aacute;cil, 1 normal, 2 dif&iacute;cil.
 * \param elegido Verdadero para el nivel elegido (imagen _P).
 */
void dibujarBotonNivel(Rectangle area, int nivel, bool elegido);

/**
 * \brief Mueve un &iacute;ndice de selecci&oacute;n dentro de un grupo de opciones, en forma
 * circular (de la &uacute;ltima se pasa a la primera y al rev&eacute;s).
 *
 * Sirve tanto para una lista vertical (KEY_DOWN/KEY_UP) como para una fila
 * horizontal (KEY_RIGHT/KEY_LEFT): qu&eacute; tecla es "avanzar" lo decide quien
 * llama, pasando las teclas que le convengan.
 *
 * \param indice           &Iacute;ndice actual dentro del grupo; se modifica aqu&iacute;.
 * \param cantidad         Cu&aacute;ntas opciones hay en el grupo.
 * \param teclaSiguiente   Tecla que avanza el &iacute;ndice.
 * \param teclaAnterior    Tecla que lo retrocede.
 */
void moverSeleccion(int& indice, int cantidad, int teclaSiguiente, int teclaAnterior);

/**
 * \brief Si el rat&oacute;n se movi&oacute; y qued&oacute; sobre alguna de las opciones, la resalta.
 *
 * As&iacute; el rat&oacute;n y el teclado controlan el mismo &iacute;ndice sin pisarse: si el
 * jugador solo usa el teclado, el rat&oacute;n nunca se movi&oacute; y esta funci&oacute;n no toca
 * nada; en cuanto se mueve el rat&oacute;n, toma el mando.
 *
 * \param areas    Rect&aacute;ngulo de cada opci&oacute;n del grupo, en el mismo orden que se
 *                 dibujan.
 * \param cantidad Cu&aacute;ntas opciones hay.
 * \param indice   &Iacute;ndice resaltado; se actualiza si el rat&oacute;n est&aacute; sobre otra opci&oacute;n.
 */
void seguirRaton(const Rectangle* areas, int cantidad, int& indice);

/**
 * \brief Si se confirm&oacute; la opci&oacute;n resaltada de un grupo.
 *
 * Se confirma con Enter o Espacio -sin importar d&oacute;nde est&eacute; el rat&oacute;n, porque ya
 * qued&oacute; resaltada por teclado-, o con un clic del rat&oacute;n encima de ella.
 *
 * \param areaResaltada Rect&aacute;ngulo de la opci&oacute;n actualmente resaltada.
 * \return Verdadero solo en el fotograma en que se confirma.
 */
bool confirmado(Rectangle areaResaltada);

#endif // BOTON_HPP_INCLUDED
