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
#include "Iconos.hpp"
#include "Tema.hpp"

// El panel mide lo mismo que fondo_FinalPartida.png ya achicado (ver
// Iconos.hpp), para que la imagen no se deforme.
static const float PANEL_ANCHO = (float)ANCHO_PANEL_FIN;
static const float PANEL_ALTO  = (float)ALTO_PANEL_FIN;

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

    // Es el mismo boton de "regresar al menu" de la pausa: mismo dibujo,
    // misma accion.
    return rectangulo(panel.x + (panel.width - ANCHO_BOTON_PAUSA) / 2.0f,
                      panel.y + panel.height - 96.0f,
                      (float)ANCHO_BOTON_PAUSA, (float)ALTO_BOTON_PAUSA);
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
    // Aqui ESC NO cierra nada. Al perder, la unica salida es el boton con el
    // mouse: asi el jugador no se salta el resumen sin querer por venir
    // picando teclas.
    if(botonClicado(botonMenu())){
        return Resultados_menu;
    }

    return Resultados_ninguna;
}

void DibujarResultados()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), COLOR_VELO);

    Rectangle panel = panelResultados();
    Texture2D fondo = fondoFin();

    // El fondo ya trae el letrero de FIN. Si falta, panel liso con el titulo
    // escrito, para que la ventana se siga entendiendo.
    if(fondo.id != 0){
        DrawTexture(fondo, (int)panel.x, (int)panel.y, WHITE);
    } else {
        DrawRectangleRounded(panel, 0.08f, 10, COLOR_FONDO);
        DrawRectangleRoundedLinesEx(panel, 0.08f, 10, 2.0f, COLOR_SELECCION);

        const char* titulo = "FIN";
        int ancho = medirTexto(titulo, 44);

        dibujarTexto(titulo,
                 (int)(panel.x + (panel.width - ancho) / 2.0f),
                 (int)(panel.y + 28.0f),
                 44, COLOR_TITULO);
    }

    // El mapache va a la izquierda, por dentro de las flores de las esquinas.
    // Si falta la imagen, el texto vuelve a ocupar todo el ancho del panel.
    Texture2D mapache = mapacheFin();

    const float MAPACHE_X = 50.0f;
    const float MAPACHE_Y = 98.0f;

    float textoX     = panel.x;
    float textoAncho = panel.width;

    if(mapache.id != 0){
        DrawTexture(mapache, (int)(panel.x + MAPACHE_X), (int)(panel.y + MAPACHE_Y), WHITE);

        // El texto se centra en lo que queda a la derecha del mapache, sin
        // llegar a las hojas de la orilla derecha.
        const float MARGEN_DERECHO = 30.0f;

        textoX     = panel.x + MAPACHE_X + LADO_MAPACHE_FIN;
        textoAncho = panel.x + panel.width - MARGEN_DERECHO - textoX;
    }

    // Centrados en su espacio y no pegados a un lado: el fondo trae flores en
    // las esquinas y por el centro es donde queda libre.
    const char* renglones[3] = {
        TextFormat("Jugador: %s", nombreFinal),
        TextFormat("Puntaje: %d", puntajeFinal),
        TextFormat("Combo mas largo: %d", mejorComboFinal)
    };

    // TextFormat rota entre 4 buferes internos, asi que guardar tres
    // resultados seguidos es seguro; un cuarto pisaria al primero.
    for(int i = 0; i < 3; i++){
        // Un poco mas chicos que antes, porque ahora comparten el ancho con el
        // mapache: con 28, "Combo mas largo" ya no cabia.
        int tamano = (i == 0) ? 22 : 26;
        int ancho  = medirTexto(renglones[i], tamano);

        dibujarTexto(renglones[i],
                 (int)(textoX + (textoAncho - ancho) / 2.0f),
                 (int)(panel.y + 108.0f + i * 44.0f),
                 tamano, COLOR_TEXTO);
    }

    // El boton se prende (_P) con el mouse encima: aqui no hay teclado.
    Rectangle boton = botonMenu();
    dibujarBotonImagen(boton, botonPausaImg(BotonPausa_menu, ratonEncima(boton)),
                       "Regresar al menu", false);
}
