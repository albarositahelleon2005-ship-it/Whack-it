/**
 * \file Instrucciones.cpp
 * \brief Implementación de la pantalla de instrucciones.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Instrucciones.hpp"
#include "Dibujo.hpp"
#include "Dificultad.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

// El texto se escribe aqui como puro texto, en un arreglo, para que agregar
// o quitar un renglon sea agregar o quitar una linea de datos. Va cortado a
// mano para que quepa en el pergamino, y sin acentos, enes ni el signo de
// admiracion de apertura: la fuente del juego solo cubre ASCII y saldrian
// cuadritos. Un renglon vacio separa los parrafos.
static const int NUM_RENGLONES = 12;

static const char* RENGLONES[NUM_RENGLONES] = {
    ""
    "Pon a prueba tus reflejos en este juego en solitario! Tu mision es",
    "atrapar a todos los mapaches que asomen por los hoyos, pero",
    "cuidado con las trampas.",
    "",
    "El jugador debera estar alerta, ya que perdera una vida si deja",
    "escapar tres mapaches o si golpea accidentalmente una bomba.",
    "",
    "Cada mapache atrapado suma 1 punto a tu marcador, pero si golpeas",
    "una bomba, perderas 2 puntos de golpe. El jugador tambien tendra",
    "que estar atento al mapache dorado! ya que es mucho mas rapido",
    "que el normal. La partida termina cuando el jugador se queda sin",
    "vidas. Puedes elegir entre tres niveles de dificultad:"
};

Escena_Estado ActualizarInstrucciones()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;
    return Escena_instrucciones;
}

void DibujarInstrucciones()
{
    // El fondo ya trae el letrero de INSTRUCCIONES. Solo si falta se escribe
    // el titulo a mano, para que la pantalla no quede sin nombre.
    Texture2D fondo = fondoInstrucciones();
    dibujarFondo(fondo);

    if(fondo.id == 0) dibujarTextoCentrado("INSTRUCCIONES", 84, 44, COLOR_TITULO);

    // Sobre el pergamino claro el texto de siempre casi no se lee: se usa el
    // cafe de las letras de madera.
    Color texto  = (fondo.id != 0) ? COLOR_TEXTO_MADERA : COLOR_TEXTO;
    Color titulo = (fondo.id != 0) ? WHITE : COLOR_SELECCION;

    // El texto va alineado a la izquierda, pero el bloque entero se centra en
    // el pergamino: se mide el renglon mas ancho y el margen sale de ahi. Con
    // un margen fijo, el renglon largo se salia por la derecha hasta la
    // madera. El centro del pergamino no es el de la ventana: la madera de la
    // izquierda es un poco mas angosta que la de la derecha.
    const int TAMANO          = 20;
    const int CENTRO_PERGAMINO = 620;

    int anchoBloque = 0;
    for(int i = 0; i < NUM_RENGLONES; i++){
        int ancho = medirTexto(RENGLONES[i], TAMANO);
        if(ancho > anchoBloque) anchoBloque = ancho;
    }

    const int MARGEN = CENTRO_PERGAMINO - anchoBloque / 2;

    const int ALTO_RENGLON = 28;

    // Un renglon debajo del letrero: pegado a el se veia muy arriba.
    const int Y_INICIO = 165 + ALTO_RENGLON;

    for(int i = 0; i < NUM_RENGLONES; i++){
        dibujarTexto(RENGLONES[i], MARGEN, Y_INICIO + i * ALTO_RENGLON, TAMANO, texto);
    }

    // Los niveles se arman desde reglasDe(), no a mano: si alguien cambia el
    // balance en Dificultad.cpp, esta pantalla se entera sola. Van un poco
    // metidos y con el nombre en otro color, como lista.
    int yNiveles = Y_INICIO + NUM_RENGLONES * ALTO_RENGLON + 14;

    for(int i = 0; i < NUM_DIFICULTADES; i++){

        ReglasDificultad reglas = reglasDe((Dificultad)i);

        const char* nombre  = TextFormat("%s:", NOMBRES_DIFICULTAD[i]);
        int         xNombre = MARGEN + 30;
        int         y       = yNiveles + i * (ALTO_RENGLON + 4);

        dibujarTexto(nombre, xNombre, y, TAMANO, titulo);

        // "1 vida" y no "1 vidas".
        dibujarTexto(TextFormat("%d hoyos y %d %s.", reglas.pozos, reglas.vidas,
                                (reglas.vidas == 1) ? "vida" : "vidas"),
                     xNombre + medirTexto(nombre, TAMANO) + 10, y, TAMANO, texto);
    }

    dibujarTexto("ESC para volver al menu",
             CENTRO_PERGAMINO - medirTexto("ESC para volver al menu", 18) / 2, 660, 18, texto);
}
