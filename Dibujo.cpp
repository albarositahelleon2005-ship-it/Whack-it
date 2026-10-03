/**
 * \file Dibujo.cpp
 * \brief Implementaci&oacute;n de las utilidades de dibujo compartidas.
 * \date 29/07/2026
 */

#include "raylib.h"

#include "Dibujo.hpp"
#include "Tema.hpp"

//***********************************************
// TIPOGRAFIA
//***********************************************

static Font fuente;
static bool hayFuente = false;

// Los glifos se rasterizan una sola vez a este tamano y despues se escalan.
// Va grande a proposito: achicar una letra se ve bien, estirarla se ve borrosa,
// y el texto mas grande del juego anda por los 50 px.
static const int TAMANO_BASE_FUENTE = 64;

void CargarFuente()
{
    // ARCO_juego.ttf es ARCO.ttf con una tabla de caracteres Unicode agregada:
    // la original solo trae la tabla "symbol" de Windows, que el lector de
    // fuentes de raylib (stb_truetype) no entiende y la rechaza completa.
    fuente = LoadFontEx("recursos/fuente/ARCO_juego.ttf", TAMANO_BASE_FUENTE, nullptr, 0);

    hayFuente = (fuente.texture.id != 0 && fuente.texture.id != GetFontDefault().texture.id);

    if(hayFuente) SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);
}

void DescargarFuente()
{
    if(hayFuente) UnloadFont(fuente);
    hayFuente = false;
}

/// Separacion entre letras: la de fabrica usa tamano/10, la nuestra ya trae aire.
static float espaciado(int tamano)
{
    return hayFuente ? tamano * 0.04f : tamano / 10.0f;
}

void dibujarTexto(const char* texto, int x, int y, int tamano, Color color)
{
    Font f = hayFuente ? fuente : GetFontDefault();
    Vector2 posicion = { (float)x, (float)y };

    DrawTextEx(f, texto, posicion, (float)tamano, espaciado(tamano), color);
}

int medirTexto(const char* texto, int tamano)
{
    Font f = hayFuente ? fuente : GetFontDefault();
    return (int)MeasureTextEx(f, texto, (float)tamano, espaciado(tamano)).x;
}

//***********************************************
// UTILIDADES
//***********************************************

void dibujarTextoCentrado(const char* texto, int y, int tamano, Color color)
{
    // medirTexto devuelve cuantos pixeles de ancho ocuparia ese texto con ese
    // tamano de fuente. Restarle la mitad al centro de la ventana lo centra.
    int ancho = medirTexto(texto, tamano);

    // GetScreenWidth se consulta aqui en vez de recibir el ancho por parametro:
    // asi este archivo no necesita saber que tamano de ventana eligio main.cpp,
    // y seguiria funcionando si algun dia la ventana se puede redimensionar.
    dibujarTexto(texto, (GetScreenWidth() - ancho) / 2, y, tamano, color);
}

void dibujarPantallaPendiente(const char* nombre)
{
    dibujarTextoCentrado(nombre, 280, 50, COLOR_TITULO);
    dibujarTextoCentrado("Esta pantalla todavia no existe", 350, 22, COLOR_TEXTO);
    dibujarTextoCentrado("ESC para volver al menu", 620, 20, COLOR_TENUE);
}

void dibujarFondo(Texture2D fondo)
{
    if(fondo.id == 0) return;

    Rectangle origen  = { 0.0f, 0.0f, (float)fondo.width, (float)fondo.height };
    Rectangle destino = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    DrawTexturePro(fondo, origen, destino, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
}
