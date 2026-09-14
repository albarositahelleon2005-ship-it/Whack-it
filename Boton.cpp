/**
 * \file Boton.cpp
 * \brief Implementaci&oacute;n de los botones con rat&oacute;n.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Boton.hpp"
#include "Tema.hpp"

Rectangle rectangulo(float x, float y, float ancho, float alto)
{
    Rectangle rec = { x, y, ancho, alto };
    return rec;
}

bool ratonEncima(Rectangle rec)
{
    return CheckCollisionPointRec(GetMousePosition(), rec);
}

bool botonClicado(Rectangle rec)
{
    // Se usa IsMouseButtonPressed y no IsMouseButtonReleased. La diferencia se
    // siente: con Pressed el boton responde en el instante en que se aprieta, y
    // con Released hasta que se suelta. Para un stand con ninos apurados, que
    // responda de inmediato vale mas que poder arrepentirse arrastrando el raton
    // fuera del boton antes de soltar.
    return ratonEncima(rec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void dibujarBoton(Rectangle rec, const char* etiqueta, bool seleccionado)
{
    // Preguntar por la posicion del raton al dibujar no rompe la separacion entre
    // actualizar y dibujar: leer donde esta el puntero no cambia nada. Lo que no
    // se vale aqui es decidir cosas, y de eso se encarga botonClicado.
    bool encima = ratonEncima(rec);

    Color fondo;
    Color textoColor;

    if(seleccionado){
        fondo      = COLOR_BOTON_ACTIVO;
        textoColor = COLOR_FONDO;          // texto oscuro sobre el boton claro
    } else if(encima){
        fondo      = COLOR_BOTON_HOVER;
        textoColor = COLOR_TEXTO;
    } else {
        fondo      = COLOR_BOTON;
        textoColor = COLOR_TEXTO;
    }

    const float REDONDEZ  = 0.25f;
    const int   SEGMENTOS = 8;

    DrawRectangleRounded(rec, REDONDEZ, SEGMENTOS, fondo);

    // El contorno solo aparece con el raton encima: marca cual se va a activar sin
    // llenar la pantalla de lineas cuando no hace falta.
    if(encima && !seleccionado){
        DrawRectangleRoundedLinesEx(rec, REDONDEZ, SEGMENTOS, 2.0f, COLOR_SELECCION);
    }

    // La letra se escala con el alto del boton para que un boton chico no se vea
    // con el texto encimado ni uno grande con el texto perdido.
    int tamano = (int)(rec.height * 0.42f);
    if(tamano < 12) tamano = 12;

    int ancho = MeasureText(etiqueta, tamano);

    DrawText(etiqueta,
             (int)(rec.x + (rec.width  - ancho ) / 2.0f),
             (int)(rec.y + (rec.height - tamano) / 2.0f),
             tamano, textoColor);
}

void dibujarBotonIcono(Rectangle rec, Texture2D textura, bool resaltado)
{
    bool encima = ratonEncima(rec);

    if(resaltado || encima){
        DrawRectangleRounded(rec, 0.3f, 8, COLOR_BOTON_HOVER);
    }

    // La textura se estira al tamano exacto de "rec": DrawTexturePro toma un
    // rectangulo de origen (la imagen completa, tal cual esta en el archivo)
    // y lo mapea al rectangulo de destino que se le pida.
    Rectangle origen = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    Vector2   sinDesfase = { 0.0f, 0.0f };

    DrawTexturePro(textura, origen, rec, sinDesfase, 0.0f, WHITE);
}

void moverSeleccion(int& indice, int cantidad, int teclaSiguiente, int teclaAnterior)
{
    if(IsKeyPressed(teclaSiguiente)) indice++;
    if(IsKeyPressed(teclaAnterior))  indice--;

    // Circular: igual que en Menu.cpp, sumar cantidad antes del modulo evita
    // el residuo negativo que da C++ con -1 % cantidad.
    indice = (indice + cantidad) % cantidad;
}

void seguirRaton(const Rectangle* areas, int cantidad, int& indice)
{
    Vector2 delta = GetMouseDelta();
    if(delta.x == 0.0f && delta.y == 0.0f) return;

    for(int i = 0; i < cantidad; i++){
        if(ratonEncima(areas[i])){
            indice = i;
            return;
        }
    }
}

bool confirmado(Rectangle areaResaltada)
{
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || botonClicado(areaResaltada);
}
