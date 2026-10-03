/**
 * \file Boton.cpp
 * \brief Implementaci&oacute;n de los botones con rat&oacute;n.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Dificultad.hpp"
#include "Iconos.hpp"
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

    int ancho = medirTexto(etiqueta, tamano);

    dibujarTexto(etiqueta,
             (int)(rec.x + (rec.width  - ancho ) / 2.0f),
             (int)(rec.y + (rec.height - tamano) / 2.0f),
             tamano, textoColor);
}

void dibujarBotonIcono(Rectangle rec, Texture2D textura, bool resaltado)
{
    // Se agranda en vez de pintarle un fondo: los iconos ya traen su propio
    // marco de madera, y un recuadro cafe detras se veia como un parche. El
    // clic se sigue revisando con el rectangulo original, asi que crecer no
    // cambia donde responde.
    if(resaltado || ratonEncima(rec)){
        const float CRECE = (float)CRECE_ICONO;
        rec = rectangulo(rec.x - CRECE, rec.y - CRECE, rec.width + CRECE * 2.0f, rec.height + CRECE * 2.0f);
    }

    // La textura se estira al tamano exacto de "rec": DrawTexturePro toma un
    // rectangulo de origen (la imagen completa, tal cual esta en el archivo)
    // y lo mapea al rectangulo de destino que se le pida.
    Rectangle origen = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    Vector2   sinDesfase = { 0.0f, 0.0f };

    DrawTexturePro(textura, origen, rec, sinDesfase, 0.0f, WHITE);
}

void dibujarBotonImagen(Rectangle rec, Texture2D textura, const char* repuesto, bool prendido)
{
    if(textura.id == 0){
        dibujarBoton(rec, repuesto, prendido);
        return;
    }

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

void dibujarBotonNivel(Rectangle area, int nivel, bool elegido)
{
    // El nivel elegido va con su imagen _P (prendida) y los demas con la _A
    // (apagada), igual que los botones del menu principal.
    Texture2D boton = botonNivel(nivel, elegido);
    Color     tono  = WHITE;

    // Si falta la version apagada pero si esta la prendida, se usa la
    // prendida oscurecida: se sigue viendo como boton de madera y no se
    // confunde con el nivel elegido.
    if(boton.id == 0 && !elegido){
        boton = botonNivel(nivel, true);
        tono  = GRAY;
    }

    if(boton.id != 0){
        DrawTexture(boton, (int)area.x, (int)area.y, tono);
    } else {
        dibujarBoton(area, NOMBRES_DIFICULTAD[nivel], elegido);
    }
}
