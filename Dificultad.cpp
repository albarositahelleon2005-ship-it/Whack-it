/**
 * \file Dificultad.cpp
 * \brief Implementación de los niveles de dificultad.
 * \date 13/09/2026
 */

#include "Dificultad.hpp"

const char* NOMBRES_DIFICULTAD[NUM_DIFICULTADES] = {
    "Facil",
    "Normal",
    "Dificil"
};

// Como se reparten los pozos en filas. Se escriben a mano, y no con una
// formula, porque son solo tres acomodos y asi se ve de un vistazo como va a
// quedar la pantalla: 3+2, 4+3 y 3+4+3.
static const int FILAS_FACIL[]   = { 3, 2 };
static const int FILAS_NORMAL[]  = { 4, 3 };
static const int FILAS_DIFICIL[] = { 3, 4, 3 };

const char* nombreDeDificultad(Dificultad nivel)
{
    return NOMBRES_DIFICULTAD[nivel];
}

ReglasDificultad reglasDe(Dificultad nivel)
{
    ReglasDificultad r;

    switch(nivel)
    {
        case Dificultad_normal:
            r.pozos           = 7;
            r.vidas           = 2;
            r.visibleInicial  = 2.0f;
            r.visibleMinimo   = 1.0f;
            r.porcentajeBomba = 25;
            r.filas           = 2;
            r.pozosPorFila    = FILAS_NORMAL;
        break;

        case Dificultad_dificil:
            r.pozos           = 10;
            r.vidas           = 1;
            r.visibleInicial  = 1.5f;
            r.visibleMinimo   = 0.5f;
            r.porcentajeBomba = 30;
            r.filas           = 3;
            r.pozosPorFila    = FILAS_DIFICIL;
        break;

        case Dificultad_facil:
        default:
            r.pozos           = 5;
            r.vidas           = 3;
            r.visibleInicial  = 3.0f;
            r.visibleMinimo   = 1.5f;
            r.porcentajeBomba = 20;
            r.filas           = 2;
            r.pozosPorFila    = FILAS_FACIL;
        break;
    }

    // La curva de aceleracion y el tope de objetos son iguales en los tres
    // niveles: lo que los distingue es de donde arranca y hasta donde baja.
    r.pasoReduccion      = 0.2f;
    r.aparicionesPorPaso = 10;
    r.maxSimultaneos     = 2;

    return r;
}
