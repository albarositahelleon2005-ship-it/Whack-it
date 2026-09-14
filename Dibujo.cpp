/**
 * \file Dibujo.cpp
 * \brief Implementaci&oacute;n de las utilidades de dibujo compartidas.
 * \date 29/07/2026
 */

#include "raylib.h"

#include "Dibujo.hpp"
#include "Tema.hpp"

void dibujarTextoCentrado(const char* texto, int y, int tamano, Color color)
{
    // MeasureText devuelve cuantos pixeles de ancho ocuparia ese texto con ese
    // tamano de fuente. Restarle la mitad al centro de la ventana lo centra.
    int ancho = MeasureText(texto, tamano);

    // GetScreenWidth se consulta aqui en vez de recibir el ancho por parametro:
    // asi este archivo no necesita saber que tamano de ventana eligio main.cpp,
    // y seguiria funcionando si algun dia la ventana se puede redimensionar.
    DrawText(texto, (GetScreenWidth() - ancho) / 2, y, tamano, color);
}

void dibujarPantallaPendiente(const char* nombre)
{
    dibujarTextoCentrado(nombre, 280, 50, COLOR_TITULO);
    dibujarTextoCentrado("Esta pantalla todavia no existe", 350, 22, COLOR_TEXTO);
    dibujarTextoCentrado("ESC para volver al menu", 620, 20, COLOR_TENUE);
}
