/**
 * \file BarraSuperior.cpp
 * \brief Implementación de la barra superior.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "BarraSuperior.hpp"
#include "Boton.hpp"
#include "Iconos.hpp"

static const float LADO   = (float)LADO_ICONO;   ///< Los íconos son cuadrados; el tamaño lo fija Iconos.hpp, que los achica al cargar.
static const float MARGEN = 20.0f;   ///< Separación con el borde de la ventana.

static Rectangle areaIconoAjustes()
{
    return rectangulo(GetScreenWidth() - MARGEN - LADO, MARGEN, LADO, LADO);
}

static Rectangle areaIconoIzquierdo()
{
    return rectangulo(MARGEN, MARGEN, LADO, LADO);
}

void dibujarBarraSuperior(IconoIzquierdo izquierdo)
{
    dibujarBotonIcono(areaIconoAjustes(), iconoAjustes(), false);

    if(izquierdo == Izq_regresar){
        dibujarBotonIcono(areaIconoIzquierdo(), iconoRegresar(), false);
    } else if(izquierdo == Izq_pausa){
        dibujarBotonIcono(areaIconoIzquierdo(), iconoPausa(), false);
    }
}

bool iconoIzquierdoClicado()
{
    return botonClicado(areaIconoIzquierdo());
}

bool iconoAjustesClicado()
{
    return botonClicado(areaIconoAjustes());
}
