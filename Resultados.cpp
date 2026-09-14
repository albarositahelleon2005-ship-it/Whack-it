/**
 * \file Resultados.cpp
 * \brief Implementación de la ventana de FIN.
 * \date 13/09/2026
 */

#include <cstring>

#include "raylib.h"

#include "Resultados.hpp"
#include "ConfigPartida.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Tema.hpp"

static const float PANEL_ANCHO = 520.0f;
static const float PANEL_ALTO  = 300.0f;

// La copia de los numeros finales. Se llena una vez, al terminar la partida.
static char nombreFinal[NOMBRE_MAX + 1] = "";
static int  puntajeFinal    = 0;
static int  mejorComboFinal = 0;

static Rectangle panelResultados()
{
    return rectangulo((GetScreenWidth()  - PANEL_ANCHO) / 2.0f,
                      (GetScreenHeight() - PANEL_ALTO ) / 2.0f,
                      PANEL_ANCHO, PANEL_ALTO);
}

static Rectangle botonMenu()
{
    Rectangle panel = panelResultados();
    const float ANCHO = 240.0f;

    return rectangulo(panel.x + (panel.width - ANCHO) / 2.0f,
                      panel.y + panel.height - 76.0f, ANCHO, 48.0f);
}

void prepararResultados(const char* nombre, int puntaje, int mejorCombo)
{
    strncpy(nombreFinal, nombre, NOMBRE_MAX);
    nombreFinal[NOMBRE_MAX] = '\0';

    puntajeFinal    = puntaje;
    mejorComboFinal = mejorCombo;
}

AccionResultados ActualizarResultados()
{
    // Aqui ESC NO cierra nada. Al perder, la unica salida es el boton: asi el
    // jugador no se salta el resumen sin querer por venir picando ESC.
    if(botonClicado(botonMenu()) || IsKeyPressed(KEY_ENTER)){
        return Resultados_menu;
    }

    return Resultados_ninguna;
}

void DibujarResultados()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), COLOR_VELO);

    Rectangle panel = panelResultados();

    DrawRectangleRounded(panel, 0.08f, 10, COLOR_PANEL);
    DrawRectangleRoundedLinesEx(panel, 0.08f, 10, 2.0f, COLOR_SELECCION);

    const char* titulo = "FIN";
    int ancho = MeasureText(titulo, 44);

    DrawText(titulo,
             (int)(panel.x + (panel.width - ancho) / 2.0f),
             (int)(panel.y + 28.0f),
             44, COLOR_TITULO);

    DrawText(TextFormat("Jugador: %s", nombreFinal),
             (int)(panel.x + 48.0f), (int)(panel.y + 100.0f), 24, COLOR_TEXTO);

    DrawText(TextFormat("Puntaje: %d", puntajeFinal),
             (int)(panel.x + 48.0f), (int)(panel.y + 138.0f), 28, COLOR_PUNTAJE);

    DrawText(TextFormat("Combo mas largo: %d", mejorComboFinal),
             (int)(panel.x + 48.0f), (int)(panel.y + 176.0f), 28, COLOR_COMBO);

    dibujarBoton(botonMenu(), "Regresar al menu", false);
}
