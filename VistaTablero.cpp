/**
 * \file VistaTablero.cpp
 * \brief Implementación del dibujo del tablero y del marcador.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "VistaTablero.hpp"
#include "Boton.hpp"
#include "Sprites.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DE LOS POZOS
//***********************************************

// El tamano del pozo es el mismo en las tres dificultades, tal como pide el
// boceto: lo unico que cambia es cuantos hay y en cuantas filas se acomodan.
static const float POZO_ANCHO = 150.0f;
static const float POZO_ALTO  =  64.0f;

static const float SEPARA_X = 40.0f;
static const float SEPARA_Y = 160.0f;

// A que altura queda el centro del conjunto de filas. Se acomoda alrededor de
// este punto para que 2 filas y 3 filas queden igual de centradas.
static const float CENTRO_Y = 420.0f;

// Que tan grande se dibuja el muneco que asoma.
static const float LADO_OBJETO = 110.0f;

// Cuanto tarda en "salir" del pozo, en segundos. Es puro adorno: no cambia
// cuando se puede golpear, solo como se ve al aparecer.
static const float DURACION_SALIDA = 0.12f;

Rectangle areaPozo(const ReglasDificultad& reglas, int indice)
{
    // Se va restando lo que lleva cada fila hasta dar con la fila del indice.
    // Con 3 filas como maximo, buscar asi es mas claro que guardar una tabla
    // de posiciones que haya que mantener en dos lados.
    int fila     = 0;
    int restante = indice;

    while(fila < reglas.filas && restante >= reglas.pozosPorFila[fila]){
        restante -= reglas.pozosPorFila[fila];
        fila++;
    }

    // Indice fuera de rango: se regresa un rectangulo vacio, que nunca va a
    // contener un clic ni se va a ver.
    if(fila >= reglas.filas) return rectangulo(0.0f, 0.0f, 0.0f, 0.0f);

    int enEstaFila = reglas.pozosPorFila[fila];

    float anchoFila = enEstaFila * POZO_ANCHO + (enEstaFila - 1) * SEPARA_X;
    float x0 = (GetScreenWidth() - anchoFila) / 2.0f;
    float y0 = CENTRO_Y - (reglas.filas - 1) * SEPARA_Y / 2.0f;

    return rectangulo(x0 + restante * (POZO_ANCHO + SEPARA_X),
                      y0 + fila * SEPARA_Y,
                      POZO_ANCHO, POZO_ALTO);
}

Rectangle areaObjeto(const ReglasDificultad& reglas, int indice)
{
    Rectangle pozo = areaPozo(reglas, indice);

    float centroX = pozo.x + pozo.width / 2.0f;

    // La base del muneco queda un poco metida en el hoyo, para que se vea que
    // sale de ahi y no que esta flotando arriba.
    float base = pozo.y + pozo.height * 0.45f;

    return rectangulo(centroX - LADO_OBJETO / 2.0f,
                      base - LADO_OBJETO,
                      LADO_OBJETO, LADO_OBJETO);
}

int pozoEn(const ReglasDificultad& reglas, Vector2 punto)
{
    for(int i = 0; i < reglas.pozos; i++){
        if(CheckCollisionPointRec(punto, areaObjeto(reglas, i))) return i;
        if(CheckCollisionPointRec(punto, areaPozo(reglas, i)))   return i;
    }

    return -1;
}

//***********************************************
// DIBUJO DEL TABLERO
//***********************************************

/**
 * \brief Dibuja el hoyo: una elipse oscura con su borde de tierra.
 */
static void dibujarHoyo(Rectangle pozo)
{
    float centroX = pozo.x + pozo.width  / 2.0f;
    float centroY = pozo.y + pozo.height / 2.0f;

    DrawEllipse((int)centroX, (int)centroY,
                pozo.width / 2.0f, pozo.height / 2.0f, COLOR_TIERRA);

    DrawEllipse((int)centroX, (int)(centroY + 3.0f),
                pozo.width / 2.0f - 8.0f, pozo.height / 2.0f - 8.0f, COLOR_POZO);
}

/**
 * \brief Dibuja lo que asoma de un pozo, con su animacion de salida.
 */
static void dibujarObjeto(const Partida& partida, int indice)
{
    const Pozo& pozo = partida.tablero.pozos[indice];
    if(pozo.contenido == Pozo_vacio) return;

    Rectangle destino = areaObjeto(partida.reglas, indice);

    // "Sale" del pozo creciendo desde abajo: se guarda donde esta la base y se
    // recalcula el rectangulo mas chico apoyado en esa misma base.
    float base = destino.y + destino.height;

    float avance = pozo.vivo / DURACION_SALIDA;
    if(avance > 1.0f) avance = 1.0f;

    float escala = 0.65f + 0.35f * avance;

    destino.width  *= escala;
    destino.height *= escala;
    destino.x       = destino.x + (LADO_OBJETO - destino.width) / 2.0f;
    destino.y       = base - destino.height;

    const Animacion& animacion = (pozo.contenido == Pozo_enemigo)
                               ? spriteEnemigo()
                               : spriteBomba();

    if(animacionLista(animacion)){
        dibujarAnimacion(animacion, destino, pozo.vivo);
        return;
    }

    // Plan B por si el .gif no estuviera: el juego se sigue pudiendo jugar con
    // un circulo de color en vez del muneco.
    Color color = (pozo.contenido == Pozo_enemigo) ? COLOR_PUNTAJE : COLOR_VIDA;

    DrawCircle((int)(destino.x + destino.width / 2.0f),
               (int)(destino.y + destino.height / 2.0f),
               destino.width / 2.5f, color);
}

void dibujarTablero(const Partida& partida)
{
    // Primero TODOS los hoyos y luego TODOS los munecos: asi ningun hoyo se
    // dibuja encima de un muneco de la fila de arriba.
    for(int i = 0; i < partida.tablero.cantidad; i++){
        dibujarHoyo(areaPozo(partida.reglas, i));
    }

    for(int i = 0; i < partida.tablero.cantidad; i++){
        dibujarObjeto(partida, i);
    }
}

//***********************************************
// MARCADOR
//***********************************************

/**
 * \brief Dibuja un corazon de vida.
 *
 * Son dos circulos y un triangulo. El orden de los vertices del triangulo
 * importa: raylib pide que vayan en sentido contrario a las manecillas -primero
 * el de arriba, luego el de la izquierda y al final el de la derecha-, o no lo
 * dibuja.
 */
static void dibujarCorazon(float centroX, float centroY, float lado, Color color)
{
    float radio = lado * 0.28f;

    DrawCircle((int)(centroX - lado * 0.22f), (int)(centroY - lado * 0.12f), radio, color);
    DrawCircle((int)(centroX + lado * 0.22f), (int)(centroY - lado * 0.12f), radio, color);

    Vector2 izquierda = { centroX - lado * 0.46f, centroY - lado * 0.04f };
    Vector2 abajo     = { centroX,                centroY + lado * 0.52f };
    Vector2 derecha   = { centroX + lado * 0.46f, centroY - lado * 0.04f };

    DrawTriangle(izquierda, abajo, derecha, color);
}

void dibujarMarcador(const Partida& partida)
{
    // Arriba a la izquierda, recorrido para no encimarse con el boton de pausa.
    DrawText(TextFormat("Jugador: %s", partida.nombre), 84, 24, 20, COLOR_TEXTO);
    DrawText(TextFormat("Puntos: %d",  partida.puntaje), 84, 52, 26, COLOR_PUNTAJE);

    // La racha va arriba a la derecha, debajo del engrane. Cuando se pierde,
    // el texto simplemente no se dibuja: no queda un "Combo 0" en pantalla.
    if(partida.combo > 0){
        const char* texto = TextFormat("Combo x%d", partida.combo);
        int ancho = MeasureText(texto, 28);

        DrawText(texto, GetScreenWidth() - 30 - ancho, 82, 28, COLOR_COMBO);
    }

    // Las vidas, abajo a la izquierda. Se dibujan TODAS las del nivel: las que
    // quedan en rosa y las gastadas en gris, para que se vea cuantas se han
    // perdido y no solo cuantas faltan.
    const float LADO_VIDA = 34.0f;
    const float SEPARA    = 44.0f;

    for(int i = 0; i < partida.reglas.vidas; i++){
        Color color = (i < partida.vidas) ? COLOR_VIDA : COLOR_VIDA_GASTADA;
        dibujarCorazon(46.0f + i * SEPARA, 668.0f, LADO_VIDA, color);
    }
}
