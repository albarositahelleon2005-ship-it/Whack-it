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

static const float PANEL_ANCHO = 420.0f;
static const float PANEL_ALTO  = 386.0f;

// Cual boton del panel esta resaltado (por teclado o por el mouse).
static int botonResaltado = 0;

/**
 * \brief El rect&aacute;ngulo del panel, centrado en la ventana.
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

    const float MARGEN   = 40.0f;
    const float ALTO     = 52.0f;
    const float SEPARA   = 18.0f;
    const float PRIMERO  = 144.0f;   // debajo del icono y del titulo

    return rectangulo(panel.x + MARGEN,
                      panel.y + PRIMERO + indice * (ALTO + SEPARA),
                      panel.width - MARGEN * 2.0f,
                      ALTO);
}

//***********************************************
// VENTANA DE PAUSA
//***********************************************

AccionPausa ActualizarPausa()
{
    // La misma tecla que abre la pausa la cierra. Si ESC hiciera otra cosa aqui
    // -por ejemplo salir al menu- seria facil perder una partida sin querer.
    if(IsKeyPressed(KEY_ESCAPE)) return Pausa_continuar;

    Rectangle areas[NUM_BOTONES_PAUSA];
    for(int i = 0; i < NUM_BOTONES_PAUSA; i++) areas[i] = botonPausa(i);

    moverSeleccion(botonResaltado, NUM_BOTONES_PAUSA, KEY_DOWN, KEY_UP);
    seguirRaton(areas, NUM_BOTONES_PAUSA, botonResaltado);

    if(confirmado(areas[botonResaltado])){
        return ACCIONES_PAUSA[botonResaltado];
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

    DrawRectangleRounded(panel, 0.08f, 10, COLOR_PANEL);
    DrawRectangleRoundedLinesEx(panel, 0.08f, 10, 2.0f, COLOR_SELECCION);

    // El mismo icono del boton que abre la pausa, arriba del titulo: asi el
    // panel se ve claramente como "lo que pasa al picar ese boton".
    const float LADO_ICONO = 44.0f;

    Texture2D icono = iconoPausa();

    Rectangle origen  = { 0.0f, 0.0f, (float)icono.width, (float)icono.height };
    Rectangle destino = { panel.x + (panel.width - LADO_ICONO) / 2.0f,
                          panel.y + 26.0f, LADO_ICONO, LADO_ICONO };
    Vector2   sinDesfase = { 0.0f, 0.0f };

    DrawTexturePro(icono, origen, destino, sinDesfase, 0.0f, COLOR_TITULO);

    const char* titulo = "PAUSA";
    int tamano = 36;
    int ancho  = MeasureText(titulo, tamano);

    DrawText(titulo,
             (int)(panel.x + (panel.width - ancho) / 2.0f),
             (int)(panel.y + 82.0f),
             tamano, COLOR_TITULO);

    for(int i = 0; i < NUM_BOTONES_PAUSA; i++){
        // Ninguno va marcado como "activo" (fondo lleno): son acciones, no
        // opciones entre las que se escoge una y se queda encendida. La que
        // esta resaltada -por teclado o por mouse- solo lleva el aro.
        dibujarBoton(botonPausa(i), ETIQUETAS_PAUSA[i], false);

        if(i == botonResaltado){
            DrawRectangleRoundedLinesEx(botonPausa(i), 0.15f, 8, 2.0f, COLOR_SELECCION);
        }
    }

    dibujarTextoCentrado("Flechas o mouse, Enter o clic     ESC para seguir jugando",
                         (int)(panel.y + panel.height - 34.0f), 16, COLOR_TENUE);
}
