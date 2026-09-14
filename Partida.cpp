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
 * \brief Vacía un pozo y, si el nivel saca los objetos de uno en uno, arranca
 * la cuenta regresiva del siguiente.
 *
 * Se pasa por aquí siempre que un pozo se libera -se haya escondido solo o lo
 * hayan golpeado-, para no repetir esa cuenta regresiva en cada caso.
 */
static void liberarPozo(Partida& partida, int indice)
{
    partida.tablero.pozos[indice].contenido = Pozo_vacio;
    partida.tablero.pozos[indice].restante  = 0.0f;
    partida.tablero.pozos[indice].vivo      = 0.0f;

    // En facil y normal, el reloj de apariciones solo corre cuando la pantalla
    // esta vacia: ese es justamente el "aparece uno hasta que desaparezca".
    // En dificil el reloj corre solo, pase lo que pase en los pozos.
    if(partida.reglas.unoALaVez){
        partida.esperaAparicion = partida.reglas.intervaloAparicion;
    }
}

/**
 * \brief Saca un objeto nuevo en algún pozo libre, y acelera un poco el juego.
 *
 * Si no hay pozos libres simplemente no saca nada: el reloj vuelve a intentarlo
 * en el siguiente intervalo.
 */
static void aparecerObjeto(Partida& partida)
{
    int indice = pozoLibreAlAzar(partida.tablero);
    if(indice < 0) return;

    Pozo& pozo = partida.tablero.pozos[indice];

    // aleatorio(1,100) <= porcentaje da exactamente esa probabilidad: con 20,
    // los numeros del 1 al 20 son bomba y del 21 al 100 son topo.
    bool esBomba = (aleatorio(1, 100) <= partida.reglas.porcentajeBomba);

    pozo.contenido = esBomba ? Pozo_bomba : Pozo_enemigo;
    pozo.restante  = partida.visibleActual;
    pozo.vivo      = 0.0f;

    // Cada aparicion deja el juego un poquito mas rapido, hasta tocar el piso
    // que puso la dificultad. De ahi en adelante se queda igual de dificil.
    partida.visibleActual -= partida.reglas.pasoReduccion;
    if(partida.visibleActual < partida.reglas.visibleMinimo){
        partida.visibleActual = partida.reglas.visibleMinimo;
    }
}

//***********************************************
// PARTIDA
//***********************************************

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
    partida.esperaAparicion = partida.reglas.intervaloAparicion;

    partida.terminada = false;
}

void avanzarPartida(Partida& partida, float dt)
{
    if(partida.terminada) return;

    // 1. Los que ya llevan su tiempo asomados se esconden.
    for(int i = 0; i < partida.tablero.cantidad; i++){

        Pozo& pozo = partida.tablero.pozos[i];
        if(pozo.contenido == Pozo_vacio) continue;

        pozo.vivo     += dt;
        pozo.restante -= dt;

        if(pozo.restante <= 0.0f){

            // Que un topo se esconda sin que lo golpearan rompe la racha, pero
            // NO quita vida. Que una bomba se esconda sola no cuesta nada: es
            // justo lo que el jugador queria que pasara.
            if(pozo.contenido == Pozo_enemigo){
                partida.combo = 0;
            }

            liberarPozo(partida, i);
        }
    }

    // 2. El reloj de apariciones.
    if(partida.reglas.unoALaVez){

        // Facil y normal: solo cuenta mientras no haya nada en pantalla.
        if(pozosOcupados(partida.tablero) == 0){
            partida.esperaAparicion -= dt;

            if(partida.esperaAparicion <= 0.0f){
                aparecerObjeto(partida);
                partida.esperaAparicion = partida.reglas.intervaloAparicion;
            }
        }

    } else {

        // Dificil: cada intervalo sale algo, haya lo que haya en pantalla.
        // Es un 'while' y no un 'if' por si un fotograma se alarga tanto que
        // le cabe mas de un intervalo; sumar en vez de reasignar evita que se
        // pierda el sobrante y el ritmo se vaya recorriendo.
        partida.esperaAparicion -= dt;

        while(partida.esperaAparicion <= 0.0f){
            aparecerObjeto(partida);
            partida.esperaAparicion += partida.reglas.intervaloAparicion;
        }
    }
}

ResultadoGolpe golpearPozo(Partida& partida, int indice)
{
    if(partida.terminada)   return Golpe_aire;
    if(indice < 0)          return Golpe_aire;
    if(indice >= partida.tablero.cantidad) return Golpe_aire;

    ContenidoPozo contenido = partida.tablero.pozos[indice].contenido;

    // Pegarle a un pozo vacio no castiga: la racha solo se pierde por la bomba
    // o por dejar escapar un topo.
    if(contenido == Pozo_vacio) return Golpe_aire;

    if(contenido == Pozo_enemigo){

        partida.puntaje += 1;
        partida.combo   += 1;

        if(partida.combo > partida.mejorCombo){
            partida.mejorCombo = partida.combo;
        }

        liberarPozo(partida, indice);
        return Golpe_enemigo;
    }

    // Lo que queda es la bomba.
    partida.puntaje -= 2;
    if(partida.puntaje < 0) partida.puntaje = 0;   // el puntaje no va a negativos

    partida.combo = 0;
    partida.vidas -= 1;

    if(partida.vidas <= 0){
        partida.vidas     = 0;
        partida.terminada = true;
    }

    liberarPozo(partida, indice);
    return Golpe_bomba;
}
