/**
 * \file VistaTablero.cpp
 * \brief Implementación del dibujo del tablero y del marcador.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "VistaTablero.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Sprites.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DE LOS POZOS
//***********************************************

// El tamano del pozo es el mismo en las tres dificultades, tal como pide el
// boceto: lo unico que cambia es cuantos hay y en cuantas filas se acomodan.
// Mide lo mismo que hoyo.png ya achicado (ver Iconos.hpp), para no deformarlo.
static const float POZO_ANCHO = (float)ANCHO_HOYO;
static const float POZO_ALTO  = (float)ALTO_HOYO;

static const float SEPARA_X = 40.0f;
static const float SEPARA_Y = 160.0f;

// A que altura queda el centro del conjunto de filas. Se acomoda alrededor de
// este punto para que 2 filas y 3 filas queden igual de centradas.
static const float CENTRO_Y = 420.0f;

// Que tan grande se dibuja el muneco que asoma. Va mas ancho que el pozo
// (150) a proposito: asi se lee bien a la distancia. Con 3 filas la cabeza
// tapa un poco el hoyo de arriba (ver el orden en dibujarTablero).
static const float LADO_OBJETO = 150.0f;

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

    // La base del muneco queda metida detras del monticulo de tierra (que se
    // dibuja encima), para que se vea que sale de adentro y no que esta
    // flotando arriba.
    float base = pozo.y + pozo.height * 0.7f;

    return rectangulo(centroX - LADO_OBJETO / 2.0f,
                      base - LADO_OBJETO,
                      LADO_OBJETO, LADO_OBJETO);
}

int pozoEn(const Partida& partida, Vector2 punto)
{
    // De atras hacia adelante: los pozos de las filas de abajo se dibujan
    // despues, encima de los de arriba, asi que si dos se enciman gana el que
    // el jugador ve al frente.
    for(int i = partida.tablero.cantidad - 1; i >= 0; i--){

        const Pozo& pozo = partida.tablero.pozos[i];
        if(pozo.contenido == Pozo_vacio || pozo.golpeado) continue;

        if(CheckCollisionPointRec(punto, areaObjeto(partida.reglas, i))) return i;
        if(CheckCollisionPointRec(punto, areaPozo(partida.reglas, i)))   return i;
    }

    return -1;
}

//***********************************************
// DIBUJO DEL TABLERO
//***********************************************

/**
 * \brief Dibuja el hoyo: el monticulo de tierra de hoyo.png.
 *
 * Si la imagen falta, una elipse oscura con su borde de tierra, como antes.
 */
static void dibujarHoyo(Rectangle pozo)
{
    Texture2D hoyo = imagenHoyo();

    if(hoyo.id != 0){
        DrawTexture(hoyo, (int)pozo.x, (int)pozo.y, WHITE);
        return;
    }

    float centroX = pozo.x + pozo.width  / 2.0f;
    float centroY = pozo.y + pozo.height / 2.0f;

    DrawEllipse((int)centroX, (int)centroY,
                pozo.width / 2.0f, pozo.height / 2.0f, COLOR_TIERRA);

    DrawEllipse((int)centroX, (int)(centroY + 3.0f),
                pozo.width / 2.0f - 8.0f, pozo.height / 2.0f - 8.0f, COLOR_POZO);
}

/**
 * \brief La imagen que toca para lo que hay en un pozo.
 */
static const Animacion& spriteDe(const Pozo& pozo)
{
    switch(pozo.contenido)
    {
        case Pozo_premium: return pozo.golpeado ? spritePremiumAplastado() : spritePremium();
        case Pozo_bomba:   return pozo.golpeado ? spriteBombaExplotada()   : spriteBomba();
        default:           return pozo.golpeado ? spriteEnemigoAplastado() : spriteEnemigo();
    }
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
    // recalcula el rectangulo mas chico apoyado en esa misma base. La imagen
    // del golpe no sale de ningun lado, aparece ya completa.
    if(!pozo.golpeado){
        float base = destino.y + destino.height;

        float avance = pozo.vivo / DURACION_SALIDA;
        if(avance > 1.0f) avance = 1.0f;

        float escala = 0.65f + 0.35f * avance;

        destino.width  *= escala;
        destino.height *= escala;
        destino.x       = destino.x + (LADO_OBJETO - destino.width) / 2.0f;
        destino.y       = base - destino.height;
    }

    const Animacion& animacion = spriteDe(pozo);

    if(animacionLista(animacion)){
        dibujarAnimacion(animacion, destino, pozo.vivo);
        return;
    }

    // Plan B por si la imagen no estuviera: el juego se sigue pudiendo jugar
    // con un circulo de color en vez del muneco. El golpeado se ve apagado.
    Color color = COLOR_PUNTAJE;
    if(pozo.contenido == Pozo_premium) color = COLOR_DORADO;
    if(pozo.contenido == Pozo_bomba)   color = COLOR_VIDA;
    if(pozo.golpeado)                  color = ColorAlpha(color, 0.35f);

    DrawCircle((int)(destino.x + destino.width / 2.0f),
               (int)(destino.y + destino.height / 2.0f),
               destino.width / 2.5f, color);
}

void dibujarTablero(const Partida& partida)
{
    // Cada pozo dibuja primero su muneco y luego su hoyo encima: el monticulo
    // de tierra tapa la parte de abajo del muneco y asi parece que sale del
    // hoyo. Se va en orden de indice, de la fila de arriba a la de abajo, para
    // que un muneco de abajo quede delante del hoyo de la fila de arriba si
    // se enciman (el mismo orden que usa pozoEn para el clic).
    for(int i = 0; i < partida.tablero.cantidad; i++){
        dibujarObjeto(partida, i);
        dibujarHoyo(areaPozo(partida.reglas, i));
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
    // Arriba a la izquierda, recorrido para no encimarse con el boton de pausa
    // (que ahora mide 96x96, el doble del tamano original).
    dibujarTexto(TextFormat("Jugador: %s", partida.nombre), 132, 24, 20, COLOR_TEXTO);
    dibujarTexto(TextFormat("Puntos: %d",  partida.puntaje), 132, 52, 26, COLOR_TEXTO);

    // La racha va arriba a la derecha, debajo del engrane. Cuando se pierde,
    // el texto simplemente no se dibuja: no queda un "Combo 0" en pantalla.
    if(partida.combo > 0){
        const char* texto = TextFormat("Combo x%d", partida.combo);
        int ancho = medirTexto(texto, 28);

        dibujarTexto(texto, GetScreenWidth() - 30 - ancho, 130, 28, COLOR_TEXTO);
    }

    // Las vidas, abajo a la izquierda. Se dibujan TODAS las del nivel: las que
    // quedan con vida.png y las gastadas con vida_perdida.png, para que se vea
    // cuantas se han perdido y no solo cuantas faltan.
    const float SEPARA = LADO_VIDA + 6.0f;
    const float CENTRO_Y = 668.0f;

    for(int i = 0; i < partida.reglas.vidas; i++){

        bool      perdida = (i >= partida.vidas);
        Texture2D icono   = iconoVida(perdida);
        float     centroX = 46.0f + i * SEPARA;

        if(icono.id != 0){
            DrawTexture(icono, (int)(centroX - LADO_VIDA / 2.0f), (int)(CENTRO_Y - LADO_VIDA / 2.0f), WHITE);
        } else {
            // Si faltan las imagenes, el corazon dibujado de antes.
            dibujarCorazon(centroX, CENTRO_Y, 34.0f, perdida ? COLOR_VIDA_GASTADA : COLOR_VIDA);
        }
    }

    // Sin este contador, perder una vida por dejar escapar topos se sentiria
    // como un error del juego: el jugador ve venir el tercer escape. Se
    // recorre a la derecha segun cuantos corazones haya.
    float xEscapes = 46.0f + partida.reglas.vidas * SEPARA;

    // Justo despues del tercer escape se muestra "3/3" resaltado un momento
    // (ver DURACION_AVISO_ESCAPES): asi se lee que ese escape fue el que costo
    // el corazon, y no que el contador se reinicio de gratis.
    bool aviso  = (partida.avisoEscapes > 0.0f);
    int  cuenta = aviso ? ESCAPES_POR_VIDA : partida.escapados % ESCAPES_POR_VIDA;

    dibujarTexto(TextFormat("Escapes: %d/%d", cuenta, ESCAPES_POR_VIDA),
             (int)xEscapes, 657, 22, aviso ? COLOR_SELECCION : COLOR_TEXTO);
}
