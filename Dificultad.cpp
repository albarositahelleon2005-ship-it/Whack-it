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
// quedar la pantalla: 3+2, 4+3 y 4+3+3.
static const int FILAS_FACIL[]   = { 3, 2 };
static const int FILAS_NORMAL[]  = { 4, 3 };
static const int FILAS_DIFICIL[] = { 4, 3, 3 };

// En cuantas apariciones se llega del tiempo visible inicial al minimo. Es el
// unico numero que decide que tan rapido se acelera el juego: subirlo hace la
// curva mas suave, bajarlo la hace mas brusca.
static const float APARICIONES_PARA_ACELERAR = 25.0f;

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
            r.pozos              = 7;
            r.vidas              = 2;
            r.visibleInicial     = 2.5f;
            r.visibleMinimo      = 1.5f;
            r.intervaloAparicion = 0.35f;   // pausa entre un objeto y el siguiente
            r.unoALaVez          = true;
            r.porcentajeBomba    = 25;
            r.filas              = 2;
            r.pozosPorFila       = FILAS_NORMAL;
        break;

        case Dificultad_dificil:
            r.pozos              = 10;
            r.vidas              = 3;
            r.visibleInicial     = 2.0f;
            r.visibleMinimo      = 1.0f;
            r.intervaloAparicion = 1.0f;    // cada segundo sale algo, sin esperar
            r.unoALaVez          = false;
            r.porcentajeBomba    = 30;
            r.filas              = 3;
            r.pozosPorFila       = FILAS_DIFICIL;
        break;

        case Dificultad_facil:
        default:
            r.pozos              = 5;
            r.vidas              = 1;
            r.visibleInicial     = 3.0f;
            r.visibleMinimo      = 1.5f;
            r.intervaloAparicion = 0.35f;
            r.unoALaVez          = true;
            r.porcentajeBomba    = 20;
            r.filas              = 2;
            r.pozosPorFila       = FILAS_FACIL;
        break;
    }

    // El paso no se escribe a mano en cada nivel: se deduce de los otros tres
    // numeros. Asi, si alguien cambia visibleInicial o visibleMinimo, la
    // aceleracion sigue cuadrando sola.
    r.pasoReduccion = (r.visibleInicial - r.visibleMinimo) / APARICIONES_PARA_ACELERAR;

    return r;
}
