/**
 * \file Pausa.cpp
 * \brief Implementación de la ventana de pausa.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Pausa.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DEL PANEL
//***********************************************

const int NUM_BOTONES_PAUSA = 3;

static const char* ETIQUETAS_PAUSA[NUM_BOTONES_PAUSA] = {
    "Continuar",
    "Reiniciar partida",
    "Regresar al menu"
};

// Cada boton lleva a una accion. Mismo truco que el menu principal: dos arreglos
// en paralelo, para que agregar una opcion sea agregar dos renglones de datos.
static const AccionPausa ACCIONES_PAUSA[NUM_BOTONES_PAUSA] = {
    Pausa_continuar,
    Pausa_reiniciar,
    Pausa_menu
};

// Que imagen le toca a cada boton, en el mismo orden que las etiquetas.
static const BotonPausa IMAGENES_PAUSA[NUM_BOTONES_PAUSA] = {
    BotonPausa_continuar,
    BotonPausa_reiniciar,
    BotonPausa_menu
};

// La pausa ya no tiene panel dibujado: solo el icono grande y los botones,
// flotando sobre el velo. El "panel" es la caja invisible que los acomoda.
static const float SEPARA_BOTONES = 20.0f;
static const float HUECO_ICONO    = 34.0f;   // entre el icono y el primer boton

static const float PANEL_ANCHO = (float)ANCHO_BOTON_PAUSA;
static const float PANEL_ALTO  = LADO_ICONO_PAUSA_GRANDE + HUECO_ICONO
                               + NUM_BOTONES_PAUSA * ALTO_BOTON_PAUSA
                               + (NUM_BOTONES_PAUSA - 1) * SEPARA_BOTONES;

// Cual boton del panel esta resaltado (por teclado o por el mouse).
static int botonResaltado = 0;

/**
 * \brief La caja (invisible) que acomoda el icono y los botones, centrada.
 *
 * Actualizar y dibujar lo calculan cada quien por su lado en vez de guardarlo. Es
 * una resta: sale m&aacute;s barato que arriesgarse a que el bot&oacute;n se dibuje en un lugar
 * y se detecte el clic en otro.
 */
static Rectangle panelPausa()
{
    return rectangulo((GetScreenWidth()  - PANEL_ANCHO) / 2.0f,
                      (GetScreenHeight() - PANEL_ALTO ) / 2.0f,
                      PANEL_ANCHO, PANEL_ALTO);
}

/**
 * \brief D&oacute;nde queda uno de los botones del panel.
 * \param indice Bot&oacute;n, desde cero.
 * \return Su rect&aacute;ngulo en pantalla.
 */
static Rectangle botonPausa(int indice)
{
    Rectangle panel = panelPausa();

    const float PRIMERO = LADO_ICONO_PAUSA_GRANDE + HUECO_ICONO;   // debajo del icono

    return rectangulo(panel.x,
                      panel.y + PRIMERO + indice * (ALTO_BOTON_PAUSA + SEPARA_BOTONES),
                      (float)ANCHO_BOTON_PAUSA,
                      (float)ALTO_BOTON_PAUSA);
}

//***********************************************
// VENTANA DE PAUSA
//***********************************************

void prepararPausa()
{
    botonResaltado = 0;
}

AccionPausa ActualizarPausa()
{
    // La misma tecla que abre la pausa la cierra. Si ESC hiciera otra cosa aqui
    // -por ejemplo salir al menu- seria facil perder una partida sin querer.
    if(IsKeyPressed(KEY_ESCAPE)) return Pausa_continuar;

    // Igual que en el menu: las flechas y el mouse mueven el mismo resaltado,
    // y Enter elige el resaltado. Aqui no se usa confirmado() porque no se
    // quiere Espacio: es facil dejarlo apretado sin querer al venir de jugar.
    Rectangle areas[NUM_BOTONES_PAUSA];
    for(int i = 0; i < NUM_BOTONES_PAUSA; i++) areas[i] = botonPausa(i);

    moverSeleccion(botonResaltado, NUM_BOTONES_PAUSA, KEY_DOWN, KEY_UP);
    seguirRaton(areas, NUM_BOTONES_PAUSA, botonResaltado);

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) return ACCIONES_PAUSA[botonResaltado];

    for(int i = 0; i < NUM_BOTONES_PAUSA; i++){
        if(botonClicado(areas[i])) return ACCIONES_PAUSA[i];
    }

    return Pausa_ninguna;
}

void DibujarPausa()
{
    // El velo cubre toda la ventana. Es semitransparente a proposito: el tablero
    // se sigue viendo debajo, y eso es lo que hace que se lea como "el juego
    // sigue ahi, en pausa" y no como "me cambiaron de pantalla".
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), COLOR_VELO);

    Rectangle panel = panelPausa();

    // El mismo icono del boton que abre la pausa, grande y arriba de los
    // botones: es lo que dice "esto es la pausa", sin titulo escrito. Es puro
    // adorno, no se puede picar.
    Texture2D icono = iconoPausa();

    if(icono.id != 0){
        DrawTexture(icono,
                    (int)(panel.x + (panel.width - LADO_ICONO_PAUSA_GRANDE) / 2.0f),
                    (int)panel.y, WHITE);
    }

    for(int i = 0; i < NUM_BOTONES_PAUSA; i++){
        // El resaltado -por flechas o mouse- va con su imagen _P (prendida), los demas
        // con la _A (apagada).
        bool resaltado = (i == botonResaltado);

        dibujarBotonImagen(botonPausa(i), botonPausaImg(IMAGENES_PAUSA[i], resaltado),
                           ETIQUETAS_PAUSA[i], resaltado);
    }
}
