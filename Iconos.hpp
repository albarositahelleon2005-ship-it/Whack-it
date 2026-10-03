/**
 * \file Iconos.hpp
 * \brief Carga las imágenes de botones y los fondos de cada pantalla.
 * \date 13/09/2026
 *
 * Las texturas se cargan una sola vez, al arrancar el juego, y se comparten
 * entre pantallas: por eso viven aquí y no dentro de Menu.cpp o Pausa.cpp.
 * Cargarlas requiere que la ventana ya esté abierta (InitWindow), así que
 * CargarIconos() se llama desde main.cpp después de InitWindow, nunca antes.
 *
 * Los botones van en recursos/botones/ y los fondos en recursos/fondo/. Para
 * agregar una imagen nueva:
 *   1. Pon el archivo .png en esa carpeta.
 *   2. Agrega una Texture2D en Iconos.cpp y cárgala en CargarIconos().
 *   3. Agrega su UnloadTexture en DescargarIconos().
 *   4. Declara aquí la función que lo devuelve.
 *
 * Si una imagen falta, su textura queda con id 0. Quien la dibuja revisa eso y
 * pone otra cosa en su lugar; el juego no se cae.
 */

#ifndef ICONOS_HPP_INCLUDED
#define ICONOS_HPP_INCLUDED

#include "raylib.h"

/**
 * \brief Carga todas las imágenes de botones. Llamar una sola vez, después
 * de InitWindow.
 */
void CargarIconos();

/**
 * \brief Libera las texturas. Llamar una sola vez, antes de CloseWindow.
 */
void DescargarIconos();

//***********************************************
// ICONOS DE LA BARRA SUPERIOR
//***********************************************

/// Lado en pantalla de los íconos cuadrados (regresar, pausa, ajustes).
const int LADO_ICONO = 96;

/// Lado del ícono grande de pausa que va arriba de los botones de la pausa.
const int LADO_ICONO_PAUSA_GRANDE = 128;

/// Cuánto crece por lado un ícono de la barra con el mouse encima.
const int CRECE_ICONO = 8;

/// Ícono de flecha, para el botón de regresar (esquina superior izquierda).
Texture2D iconoRegresar();

/// Ícono de dos barras, para pausar durante la partida y para el panel de pausa.
Texture2D iconoPausa();

/// Ícono de engrane, para abrir la ventana de ajustes (esquina superior derecha).
Texture2D iconoAjustes();

//***********************************************
// FONDOS
//***********************************************

/**
 * \brief Imagen de fondo del menú principal (recursos/fondo/).
 *
 * No es un botón, pero vive aquí por la misma razón que los íconos: se carga
 * una sola vez después de InitWindow. Si el archivo falta, la textura queda
 * con id 0 y el menú se queda con el color de fondo de siempre.
 */
Texture2D fondoPrincipal();

/// Fondo de la partida. Mismas reglas que fondoPrincipal().
Texture2D fondoJuego();

/// Fondo de la configuración de partida. Mismas reglas que fondoPrincipal().
Texture2D fondoConfiguracion();

/// Fondo de las instrucciones; ya trae dibujado el título. Mismas reglas que fondoPrincipal().
Texture2D fondoInstrucciones();

/// Fondo de los créditos; ya trae dibujado el título. Mismas reglas que fondoPrincipal().
Texture2D fondoCreditos();

/// Fondo de los mejores puntajes; ya trae dibujado el título. Mismas reglas que fondoPrincipal().
Texture2D fondoPuntaje();

//***********************************************
// BOTONES DE MADERA
//***********************************************

/// Tamaño en pantalla de los botones de madera del menú principal. Respeta la
/// proporción de los PNG originales (10834x1917, casi 5.65 : 1).
const int ANCHO_BOTON_INTRO = 340;
const int ALTO_BOTON_INTRO  = 60;

/**
 * \brief Botón de madera del menú principal (recursos/botones/boton_intro_*).
 *
 * \param indice   Opción del menú, en el orden de Menu.cpp: 0 jugar, 1 puntaje,
 *                 2 instrucciones, 3 créditos, 4 salir.
 * \param prendido Verdadero para la versión resaltada (_P), falso para la
 *                 apagada (_A).
 * \return La textura; con id 0 si el archivo falta o el índice no existe.
 */
Texture2D botonIntro(int indice, bool prendido);

/// Tamaño en pantalla de la tabla donde se escribe el nombre (PNG de 17500x2167).
const int ANCHO_MARCO_NOMBRE = 486;
const int ALTO_MARCO_NOMBRE  = 60;

/// Tamaño en pantalla de los botones de nivel (PNG de 7500x2167).
const int ANCHO_BOTON_NIVEL = 200;
const int ALTO_BOTON_NIVEL  = 58;

/// Tabla de madera que va detrás del nombre del jugador en la configuración.
Texture2D marcoNombre();

/// Tamaño del parche de tierra de la vista previa de pozos. Respeta la
/// proporción del PNG (1059x570, casi 1.86 : 1): se fija el alto, que pidió
/// el equipo, y el ancho sale de ahí.
const int ALTO_TIERRA_CONFIG  = 200;
const int ANCHO_TIERRA_CONFIG = ALTO_TIERRA_CONFIG * 1059 / 570;

/// Parche de tierra bajo los niveles, donde va la vista previa de los pozos
/// (recursos/fondo/fondo_configuracion_tierra.png).
Texture2D fondoTierraConfig();

/// Tamaño en pantalla del botón de inicio de la configuración (PNG de 8334x2500).
const int ANCHO_BOTON_INICIO = 200;
const int ALTO_BOTON_INICIO  = 60;

/**
 * \brief Botón de madera "Inicio" de la configuración (recursos/botones/boton_configuracion_inicio_*).
 * \param prendido Verdadero para la versión resaltada (_P).
 */
Texture2D botonInicio(bool prendido);

//***********************************************
// PARTIDA
//***********************************************

/// Lado en pantalla de cada corazón del marcador.
const int LADO_VIDA = 44;

/// Tamaño del hoyo de cada pozo en la partida (PNG de proporción 2.34 : 1).
const int ANCHO_HOYO = 150;
const int ALTO_HOYO  = 64;

/**
 * \brief Montículo de tierra de un pozo (recursos/imagenes/hoyo.png).
 *
 * Es solo el borde de enfrente del hoyo: se dibuja **encima** del topo para
 * que se vea que sale de adentro.
 */
Texture2D imagenHoyo();

/**
 * \brief Corazón del marcador (recursos/imagenes/vida*.png).
 * \param perdida Verdadero para el corazón roto de una vida ya gastada.
 */
Texture2D iconoVida(bool perdida);

//***********************************************
// VENTANAS VIRTUALES (pausa, ajustes, fin)
//***********************************************

/// Tamaño de los botones de la pausa (PNG de proporción 6.54 : 1). También
/// se usa el de "regresar al menú" en la ventana de FIN.
const int ANCHO_BOTON_PAUSA = 360;
const int ALTO_BOTON_PAUSA  = 55;

/// Botones de la ventana de pausa, en el orden de Pausa.cpp.
enum BotonPausa {
    BotonPausa_continuar,
    BotonPausa_reiniciar,
    BotonPausa_menu,
    NUM_BOTONES_PAUSA_IMG
};

/**
 * \brief Botón de madera de la pausa (recursos/botones/boton_pausa_*).
 * \param cual     Cuál de los tres.
 * \param prendido Verdadero para la versión resaltada (_P).
 */
Texture2D botonPausaImg(BotonPausa cual, bool prendido);

/// Tamaño del panel de ajustes; respeta la proporción de fondo_ajustes.png.
const int ANCHO_PANEL_AJUSTES = 600;
const int ALTO_PANEL_AJUSTES  = 412;

/// Tamaño de la ventana de FIN; respeta la proporción de fondo_FinalPartida.png.
const int ANCHO_PANEL_FIN = 640;
const int ALTO_PANEL_FIN  = 369;

/// Lado de los botones de subir y bajar volumen.
const int LADO_BOTON_VOLUMEN = 48;

/// Tamaño del botón de cerrar los ajustes (PNG de proporción 4.35 : 1).
const int ANCHO_BOTON_CERRAR = 200;
const int ALTO_BOTON_CERRAR  = 46;

/// Fondo de la ventana de ajustes; ya trae el título AJUSTES.
Texture2D fondoAjustes();

/// Fondo de la ventana de FIN; ya trae el título FIN.
Texture2D fondoFin();

/// Lado del mapache que acompaña los resultados en la ventana de FIN.
const int LADO_MAPACHE_FIN = 160;

/// El mapache de la ventana de FIN (recursos/imagenes/mapache_baka.png).
Texture2D mapacheFin();

/// Botón "+" de subir el volumen.
Texture2D botonVolumenMas();

/// Botón "-" de bajar el volumen.
Texture2D botonVolumenMenos();

/// Botón de cerrar los ajustes. \param prendido Verdadero con el mouse encima (_P).
Texture2D botonCerrarImg(bool prendido);

/**
 * \brief Botón de un nivel de dificultad (recursos/botones/boton_configuracion_nivel_*).
 *
 * \param indice   Nivel, en el orden de Dificultad.hpp: 0 fácil, 1 normal, 2 difícil.
 * \param prendido Verdadero para el nivel elegido (_P), falso para los demás (_A).
 * \return La textura; con id 0 si el archivo falta o el índice no existe.
 */
Texture2D botonNivel(int indice, bool prendido);

#endif // ICONOS_HPP_INCLUDED
