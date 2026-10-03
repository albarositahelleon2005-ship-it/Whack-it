/**
 * \file Partida.cpp
 * \brief Implementación de las reglas de la partida.
 * \date 13/09/2026
 */

#include <cstring>

#include "Partida.hpp"
#include "Aleatorio.hpp"

//***********************************************
// AYUDANTES INTERNOS
//***********************************************

/**
 * \brief Cada cuánto sale un objeto nuevo.
 *
 * Se amarra al tiempo visible en vez de ser un número fijo: si un objeto dura T
 * segundos y sale uno cada T / maxSimultaneos, en pantalla se juntan justo
 * hasta el tope. Así, cuando el juego acelera, también acelera el ritmo de
 * apariciones sin tener que ajustar un segundo número.
 */
static float intervaloAparicion(const Partida& partida)
{
    return partida.visibleActual / (float)partida.reglas.maxSimultaneos;
}

static void quitarVida(Partida& partida)
{
    partida.vidas -= 1;

    if(partida.vidas <= 0){
        partida.vidas     = 0;
        partida.terminada = true;
    }
}

/**
 * \brief Deja el objeto del pozo como "aplastado" un momento.
 *
 * El pozo sigue ocupado mientras se ve la imagen del golpe: así no puede salir
 * otro objeto encima, y ese objeto cuenta para el tope de simultáneos.
 */
static void marcarGolpeado(Pozo& pozo)
{
    pozo.golpeado = true;
    pozo.restante = DURACION_APLASTADO;
    pozo.vivo     = 0.0f;
}

/**
 * \brief Saca un objeto nuevo en algún pozo libre, y acelera un poco el juego.
 */
static void aparecerObjeto(Partida& partida)
{
    int indice = pozoLibreAlAzar(partida.tablero);
    if(indice < 0) return;

    Pozo& pozo = partida.tablero.pozos[indice];

    // Primero se decide si toca dorado; solo si no, se tira bomba contra topo.
    // Al reves, el porcentaje de bomba se comeria parte del 15% del dorado.
    if(aleatorio(1, 100) <= porcentajePremium(partida.sinPremium)){
        pozo.contenido     = Pozo_premium;
        partida.sinPremium = 0;
    } else {
        // aleatorio(1,100) <= porcentaje da exactamente esa probabilidad: con
        // 20, los numeros del 1 al 20 son bomba y del 21 al 100 son topo.
        bool esBomba = (aleatorio(1, 100) <= partida.reglas.porcentajeBomba);

        pozo.contenido = esBomba ? Pozo_bomba : Pozo_enemigo;
        partida.sinPremium++;
    }

    pozo.golpeado = false;
    pozo.restante = partida.visibleActual;
    pozo.vivo     = 0.0f;

    // El juego no acelera en cada aparicion sino a saltos: cada tantas
    // apariciones se recorta el tiempo visible, hasta tocar el piso del nivel.
    partida.apariciones++;

    if(partida.apariciones % partida.reglas.aparicionesPorPaso == 0){
        partida.visibleActual -= partida.reglas.pasoReduccion;

        if(partida.visibleActual < partida.reglas.visibleMinimo){
            partida.visibleActual = partida.reglas.visibleMinimo;
        }
    }
}

//***********************************************
// PARTIDA
//***********************************************

int porcentajePremium(int sinPremium)
{
    // sinPremium cuenta los que YA salieron, asi que el que esta por salir es
    // el numero sinPremium + 1.
    int siguiente = sinPremium + 1;

    if(siguiente >= PREMIUM_SEGURO_EN)     return 100;
    if(sinPremium >= PREMIUM_TARDE_DESPUES) return PORCENTAJE_PREMIUM_TARDE;
    return PORCENTAJE_PREMIUM;
}

void iniciarPartida(Partida& partida, const ConfigPartida& config)
{
    partida.reglas = reglasDe(config.dificultad);

    vaciarTablero(partida.tablero, partida.reglas.pozos);

    // strncpy no agrega el '\0' si la cadena de origen llena todo el espacio,
    // asi que se pone a mano. Es el clasico detalle de las cadenas de C.
    strncpy(partida.nombre, config.nombre, NOMBRE_MAX);
    partida.nombre[NOMBRE_MAX] = '\0';

    partida.puntaje    = 0;
    partida.vidas      = partida.reglas.vidas;
    partida.combo      = 0;
    partida.mejorCombo = 0;

    partida.visibleActual   = partida.reglas.visibleInicial;
    partida.esperaAparicion = intervaloAparicion(partida);
    partida.apariciones     = 0;
    partida.sinPremium      = 0;
    partida.escapados       = 0;
    partida.avisoEscapes    = 0.0f;

    partida.terminada = false;
}

void avanzarPartida(Partida& partida, float dt)
{
    if(partida.terminada) return;

    if(partida.avisoEscapes > 0.0f) partida.avisoEscapes -= dt;

    // 1. Los que ya llevan su tiempo asomados se esconden.
    for(int i = 0; i < partida.tablero.cantidad; i++){

        Pozo& pozo = partida.tablero.pozos[i];
        if(pozo.contenido == Pozo_vacio) continue;

        pozo.vivo     += dt;
        pozo.restante -= dt;

        if(pozo.restante > 0.0f) continue;

        // Un topo que se esconde sin que lo golpearan rompe la racha, y cada
        // ESCAPES_POR_VIDA escapes (en toda la partida, no seguidos) cuestan una
        // vida. Que una bomba se esconda sola no cuesta nada: es justo lo que el
        // jugador queria que pasara.
        bool esTopo = (pozo.contenido == Pozo_enemigo || pozo.contenido == Pozo_premium);

        if(esTopo && !pozo.golpeado){
            partida.combo = 0;
            partida.escapados++;

            // Un escape nuevo corta el aviso de "3/3" del anterior: el
            // marcador tiene que mostrar ya la cuenta de verdad.
            partida.avisoEscapes = 0.0f;

            if(partida.escapados % ESCAPES_POR_VIDA == 0){
                quitarVida(partida);
                partida.avisoEscapes = DURACION_AVISO_ESCAPES;
            }
        }

        vaciarPozo(pozo);
    }

    if(partida.terminada) return;

    // 2. El reloj de apariciones. Si ya esta el tope de objetos en pantalla, la
    //    cuenta se queda en cero y el siguiente sale en cuanto se libere un pozo.
    partida.esperaAparicion -= dt;

    if(partida.esperaAparicion <= 0.0f){

        if(pozosOcupados(partida.tablero) < partida.reglas.maxSimultaneos){
            aparecerObjeto(partida);
            partida.esperaAparicion = intervaloAparicion(partida);
        } else {
            partida.esperaAparicion = 0.0f;
        }
    }
}

ResultadoGolpe golpearPozo(Partida& partida, int indice)
{
    if(partida.terminada)   return Golpe_aire;
    if(indice < 0)          return Golpe_aire;
    if(indice >= partida.tablero.cantidad) return Golpe_aire;

    Pozo& pozo = partida.tablero.pozos[indice];

    // Pegarle a un pozo vacio -o a uno que ya se golpeo y solo esta mostrando
    // la imagen del golpe- no castiga ni suma.
    if(pozo.contenido == Pozo_vacio || pozo.golpeado) return Golpe_aire;

    if(pozo.contenido == Pozo_enemigo || pozo.contenido == Pozo_premium){

        bool premium = (pozo.contenido == Pozo_premium);

        partida.puntaje += premium ? PUNTOS_PREMIUM : 1;
        partida.combo   += 1;

        if(partida.combo > partida.mejorCombo){
            partida.mejorCombo = partida.combo;
        }

        marcarGolpeado(pozo);
        return premium ? Golpe_premium : Golpe_enemigo;
    }

    // Lo que queda es la bomba.
    partida.puntaje -= 2;
    if(partida.puntaje < 0) partida.puntaje = 0;   // el puntaje no va a negativos

    partida.combo = 0;
    quitarVida(partida);

    marcarGolpeado(pozo);
    return Golpe_bomba;
}
