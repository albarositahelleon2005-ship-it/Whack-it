/**
 * \file Tablero.cpp
 * \brief Implementación del área de juego.
 * \date 13/09/2026
 */

#include "Tablero.hpp"
#include "Aleatorio.hpp"

void vaciarTablero(Tablero& tablero, int cantidad)
{
    if(cantidad < 1)         cantidad = 1;
    if(cantidad > MAX_POZOS) cantidad = MAX_POZOS;

    tablero.cantidad = cantidad;

    // Se limpian TODOS los pozos, no solo los que se van a usar: asi no queda
    // basura del tamano de tablero anterior si el jugador cambia de dificultad
    // y vuelve a jugar.
    for(int i = 0; i < MAX_POZOS; i++){
        tablero.pozos[i].contenido = Pozo_vacio;
        tablero.pozos[i].restante  = 0.0f;
        tablero.pozos[i].vivo      = 0.0f;
    }
}

int pozosOcupados(const Tablero& tablero)
{
    int cuenta = 0;

    for(int i = 0; i < tablero.cantidad; i++){
        if(tablero.pozos[i].contenido != Pozo_vacio) cuenta++;
    }

    return cuenta;
}

int pozoLibreAlAzar(const Tablero& tablero)
{
    int libres[MAX_POZOS];
    int cuantos = 0;

    for(int i = 0; i < tablero.cantidad; i++){
        if(tablero.pozos[i].contenido == Pozo_vacio){
            libres[cuantos] = i;
            cuantos++;
        }
    }

    if(cuantos == 0) return -1;

    return libres[aleatorio(0, cuantos - 1)];
}
