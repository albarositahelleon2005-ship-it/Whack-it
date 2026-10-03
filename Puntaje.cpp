/**
 * \file Puntaje.cpp
 * \brief Implementación de la pantalla de mejores puntajes.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Puntaje.hpp"
#include "TablaPuntajes.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

// Junto a la carpeta recursos/, que es donde main() deja la carpeta de
// trabajo. Es texto: se puede abrir con el Bloc de notas (ver TablaPuntajes.hpp).
static const char* ARCHIVO_PUNTAJES = "puntajes.txt";

static TablaPuntajes tabla;

// Que nivel se esta viendo. Se queda entre visitas a la pantalla, y al
// terminar una partida cambia a ese nivel, para que el jugador se busque ahi.
static int nivelMostrado = Dificultad_facil;

//***********************************************
// ACOMODO DE LA PANTALLA
//***********************************************

// Todo va sobre la hoja del fondo, que no esta centrada en la ventana: a la
// derecha tiene la pasta del cuaderno y el cafe. Este es el centro de la hoja.
static const float CENTRO_HOJA = 660.0f;

static const float FILA_BOTONES  = 200.0f;
static const float SEPARA_BOTON  =  16.0f;

// Columnas de la tabla. Los encabezados largos van en dos renglones para que
// las tres columnas quepan en la hoja sin encimarse.
static const int   X_LUGAR        = 390;   // el numero de lugar se alinea a la derecha aqui
static const int   X_NOMBRE       = 410;
static const int   CENTRO_RACHA   = 770;
static const int   CENTRO_MAPACHES = 925;

static const int   Y_ENCABEZADO   = 278;
static const int   Y_PRIMER_LUGAR = 340;
static const int   ALTO_RENGLON   =  31;
static const int   TAMANO_TABLA   =  22;

/// El boton de un nivel.
static Rectangle botonNivelPuntaje(int nivel)
{
    float anchoFila = NUM_DIFICULTADES * ANCHO_BOTON_NIVEL + (NUM_DIFICULTADES - 1) * SEPARA_BOTON;
    float x0        = CENTRO_HOJA - anchoFila / 2.0f;

    return rectangulo(x0 + nivel * (ANCHO_BOTON_NIVEL + SEPARA_BOTON), FILA_BOTONES,
                      (float)ANCHO_BOTON_NIVEL, (float)ALTO_BOTON_NIVEL);
}

/// Dibuja un texto centrado en una x, no en la ventana.
static void textoCentradoEn(const char* texto, int centroX, int y, int tamano, Color color)
{
    dibujarTexto(texto, centroX - medirTexto(texto, tamano) / 2, y, tamano, color);
}

//***********************************************
// TABLA
//***********************************************

void CargarPuntajes()
{
    cargarTabla(tabla, ARCHIVO_PUNTAJES);
}

void anotarPuntaje(Dificultad nivel, const char* nombre, int puntos, int mejorRacha)
{
    anotarEnTabla(tabla, nivel, nombre, puntos, mejorRacha);

    // Si no se puede escribir (carpeta de solo lectura), la tabla sigue
    // valiendo mientras el juego este abierto; solo se pierde al cerrarlo.
    guardarTabla(tabla, ARCHIVO_PUNTAJES);

    nivelMostrado = nivel;
}

//***********************************************
// PANTALLA
//***********************************************

Escena_Estado ActualizarPuntaje()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;

    // Sin vuelta circular, igual que los niveles de la configuracion: de
    // Dificil a la derecha no se brinca a Facil.
    if(IsKeyPressed(KEY_RIGHT) && nivelMostrado < NUM_DIFICULTADES - 1) nivelMostrado++;
    if(IsKeyPressed(KEY_LEFT)  && nivelMostrado > 0)                    nivelMostrado--;

    for(int i = 0; i < NUM_DIFICULTADES; i++){
        if(botonClicado(botonNivelPuntaje(i))) nivelMostrado = i;
    }

    return Escena_puntajes;
}

void DibujarPuntaje()
{
    // El fondo ya trae el letrero de PUNTAJE. Solo si falta se escribe a mano.
    Texture2D fondo = fondoPuntaje();
    dibujarFondo(fondo);

    if(fondo.id == 0) dibujarTextoCentrado("MEJORES PUNTAJES", 84, 44, COLOR_TITULO);

    for(int i = 0; i < NUM_DIFICULTADES; i++){
        dibujarBotonNivel(botonNivelPuntaje(i), i, i == nivelMostrado);
    }

    // Encabezados. La columna de mapaches va en naranja en toda la tabla:
    // es el numero que importa, y asi se encuentra de un vistazo.
    const int TAMANO_ENCABEZADO = 20;
    const int RENGLON_2         = Y_ENCABEZADO + 24;

    dibujarTexto("Nombre", X_NOMBRE, RENGLON_2, TAMANO_ENCABEZADO, COLOR_TEXTO_MADERA);

    textoCentradoEn("Racha",     CENTRO_RACHA, Y_ENCABEZADO, TAMANO_ENCABEZADO, COLOR_TEXTO_MADERA);
    textoCentradoEn("mas larga", CENTRO_RACHA, RENGLON_2,    TAMANO_ENCABEZADO, COLOR_TEXTO_MADERA);

    textoCentradoEn("Mapaches",  CENTRO_MAPACHES, Y_ENCABEZADO, TAMANO_ENCABEZADO, COLOR_NARANJA);
    textoCentradoEn("atrapados", CENTRO_MAPACHES, RENGLON_2,    TAMANO_ENCABEZADO, COLOR_NARANJA);

    // Raya bajo los encabezados, del lugar a la ultima columna.
    int yRaya = RENGLON_2 + TAMANO_ENCABEZADO + 8;
    DrawLineEx(Vector2{ (float)(X_LUGAR - 40), (float)yRaya },
               Vector2{ (float)(CENTRO_MAPACHES + 80), (float)yRaya },
               2.0f, ColorAlpha(COLOR_TEXTO_MADERA, 0.5f));

    int cantidad = tabla.cantidad[nivelMostrado];

    if(cantidad == 0){
        textoCentradoEn("Todavia no hay puntajes en este nivel", (int)CENTRO_HOJA,
                        Y_PRIMER_LUGAR + 60, TAMANO_TABLA, COLOR_TENUE);
    }

    for(int i = 0; i < cantidad; i++){
        const RegistroPuntaje& r = tabla.lugares[nivelMostrado][i];
        int y = Y_PRIMER_LUGAR + i * ALTO_RENGLON;

        const char* lugar = TextFormat("%d.", i + 1);
        dibujarTexto(lugar, X_LUGAR - medirTexto(lugar, TAMANO_TABLA), y, TAMANO_TABLA, COLOR_TEXTO_MADERA);

        dibujarTexto(r.nombre, X_NOMBRE, y, TAMANO_TABLA, COLOR_TEXTO_MADERA);

        textoCentradoEn(TextFormat("%d", r.mejorRacha), CENTRO_RACHA,    y, TAMANO_TABLA, COLOR_TEXTO_MADERA);
        textoCentradoEn(TextFormat("%d", r.puntos),     CENTRO_MAPACHES, y, TAMANO_TABLA, COLOR_NARANJA);
    }

    textoCentradoEn("Flechas para cambiar de nivel  -  ESC para volver al menu",
                    (int)CENTRO_HOJA, 680, 18, COLOR_TENUE);
}
