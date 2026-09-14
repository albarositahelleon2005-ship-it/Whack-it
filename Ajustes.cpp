/**
 * \file Ajustes.cpp
 * \brief Implementación de la ventana de ajustes.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Ajustes.hpp"
#include "Audio.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DEL PANEL
//***********************************************

static const float PANEL_ANCHO = 480.0f;
static const float PANEL_ALTO  = 330.0f;

static const float MARGEN     = 36.0f;
static const float LADO_BOTON = 40.0f;

// A que altura, ya dentro del panel, empieza cada renglon de volumen.
static const float FILA_MUSICA  = 108.0f;
static const float FILA_EFECTOS = 192.0f;

static bool abierta = false;

/**
 * \brief El rectangulo del panel, centrado en la ventana.
 *
 * Actualizar y dibujar lo calculan cada quien por su lado en vez de guardarlo:
 * es una resta, y sale mas barato que arriesgarse a que un boton se dibuje en
 * un lugar y se detecte el clic en otro.
 */
static Rectangle panelAjustes()
{
    return rectangulo((GetScreenWidth()  - PANEL_ANCHO) / 2.0f,
                      (GetScreenHeight() - PANEL_ALTO ) / 2.0f,
                      PANEL_ANCHO, PANEL_ALTO);
}

/// El boton de bajar volumen de un renglon.
static Rectangle botonMenos(float fila)
{
    Rectangle panel = panelAjustes();
    return rectangulo(panel.x + MARGEN, panel.y + fila, LADO_BOTON, LADO_BOTON);
}

/// El boton de subir volumen de un renglon.
static Rectangle botonMas(float fila)
{
    Rectangle panel = panelAjustes();
    return rectangulo(panel.x + panel.width - MARGEN - LADO_BOTON,
                      panel.y + fila, LADO_BOTON, LADO_BOTON);
}

/// La barra que muestra en cuanto va el volumen, entre los dos botones.
static Rectangle barraVolumen(float fila)
{
    Rectangle menos = botonMenos(fila);
    Rectangle mas   = botonMas(fila);

    const float HUECO = 12.0f;
    float inicio = menos.x + menos.width + HUECO;

    return rectangulo(inicio, menos.y + 8.0f, mas.x - HUECO - inicio, LADO_BOTON - 16.0f);
}

/// El boton de cerrar la ventana.
static Rectangle botonCerrar()
{
    Rectangle panel = panelAjustes();
    const float ANCHO = 200.0f;

    return rectangulo(panel.x + (panel.width - ANCHO) / 2.0f,
                      panel.y + panel.height - 76.0f, ANCHO, 46.0f);
}

/**
 * \brief Dibuja un renglon completo: etiqueta, boton de bajar, barra y boton
 * de subir.
 */
static void dibujarRenglon(const char* etiqueta, float fila, float nivel)
{
    Rectangle panel = panelAjustes();

    DrawText(etiqueta, (int)(panel.x + MARGEN), (int)(panel.y + fila - 28.0f), 20, COLOR_TEXTO);

    dibujarBoton(botonMenos(fila), "-", false);
    dibujarBoton(botonMas(fila),   "+", false);

    Rectangle barra = barraVolumen(fila);

    DrawRectangleRounded(barra, 0.5f, 8, COLOR_BOTON);

    // La parte llena es la misma barra pero con el ancho recortado al nivel.
    // Con volumen en cero no se dibuja nada: un rectangulo de ancho cero se
    // vería como una rayita suelta por el redondeo.
    if(nivel > 0.0f){
        Rectangle llena = barra;
        llena.width = barra.width * nivel;
        DrawRectangleRounded(llena, 0.5f, 8, COLOR_SELECCION);
    }

    // El porcentaje va a la derecha de la barra, en numero, porque de un
    // vistazo la barra dice "como va" pero no dice "cuanto es".
    const char* texto = TextFormat("%d%%", (int)(nivel * 100.0f + 0.5f));
    int ancho = MeasureText(texto, 18);

    DrawText(texto,
             (int)(barra.x + barra.width - ancho - 10.0f),
             (int)(panel.y + fila - 28.0f),
             18, COLOR_TENUE);
}

//***********************************************
// VENTANA DE AJUSTES
//***********************************************

void abrirAjustes()
{
    abierta = true;
}

bool ajustesAbiertos()
{
    return abierta;
}

void ActualizarAjustes()
{
    if(!abierta) return;

    if(IsKeyPressed(KEY_ESCAPE)){
        abierta = false;
        return;
    }

    if(botonClicado(botonMenos(FILA_MUSICA)))  ajustarVolumenMusica(-PASO_VOLUMEN);
    if(botonClicado(botonMas(FILA_MUSICA)))    ajustarVolumenMusica( PASO_VOLUMEN);

    if(botonClicado(botonMenos(FILA_EFECTOS))) ajustarVolumenEfectos(-PASO_VOLUMEN);
    if(botonClicado(botonMas(FILA_EFECTOS)))   ajustarVolumenEfectos( PASO_VOLUMEN);

    if(botonClicado(botonCerrar())) abierta = false;
}

void DibujarAjustes()
{
    if(!abierta) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), COLOR_VELO);

    Rectangle panel = panelAjustes();

    DrawRectangleRounded(panel, 0.08f, 10, COLOR_PANEL);
    DrawRectangleRoundedLinesEx(panel, 0.08f, 10, 2.0f, COLOR_SELECCION);

    const char* titulo = "AJUSTES";
    int ancho = MeasureText(titulo, 34);

    DrawText(titulo,
             (int)(panel.x + (panel.width - ancho) / 2.0f),
             (int)(panel.y + 32.0f),
             34, COLOR_TITULO);

    dibujarRenglon("Musica",       FILA_MUSICA,  volumenMusica());
    dibujarRenglon("Sonidos (SFX)", FILA_EFECTOS, volumenEfectos());

    dibujarBoton(botonCerrar(), "Cerrar", false);
}
