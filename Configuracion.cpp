/**
 * \file Configuracion.cpp
 * \brief Implementación de la pantalla de configuración de partida.
 * \date 13/09/2026
 */

#include <cstring>

#include "raylib.h"

#include "Configuracion.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DE LA PANTALLA
//***********************************************

static const float ETIQUETA_X = 200.0f;
static const float CAMPO_X    = 360.0f;

static const float FILA_NOMBRE = 178.0f;
static const float FILA_NIVEL   = 262.0f;

static const float BOTON_NIVEL_ANCHO = 180.0f;
static const float BOTON_NIVEL_ALTO  =  54.0f;
static const float SEPARA_NIVEL      =  20.0f;

/// El rectángulo donde se escribe el nombre.
static Rectangle campoNombre()
{
    return rectangulo(CAMPO_X, FILA_NOMBRE, 420.0f, 52.0f);
}

/// El botón de un nivel de dificultad.
static Rectangle botonDificultad(int indice)
{
    return rectangulo(CAMPO_X + indice * (BOTON_NIVEL_ANCHO + SEPARA_NIVEL),
                      FILA_NIVEL, BOTON_NIVEL_ANCHO, BOTON_NIVEL_ALTO);
}

/// El botón de empezar a jugar.
static Rectangle botonIniciar()
{
    return rectangulo(GetScreenWidth() - 260.0f, 600.0f, 200.0f, 60.0f);
}

/// La caja donde se muestra cómo van a quedar los pozos.
static Rectangle cajaVistaPrevia()
{
    return rectangulo(ETIQUETA_X, 370.0f, GetScreenWidth() - ETIQUETA_X * 2.0f, 200.0f);
}

//***********************************************
// NOMBRE DEL JUGADOR
//***********************************************

/**
 * \brief Si el nombre escrito sirve para empezar.
 *
 * No basta con que tenga letras: un nombre de puros espacios se veria vacio en
 * el marcador, asi que se pide al menos un caracter que no sea espacio.
 */
static bool nombreValido(const char* nombre)
{
    for(int i = 0; nombre[i] != '\0'; i++){
        if(nombre[i] != ' ') return true;
    }

    return false;
}

/**
 * \brief Mete en el nombre lo que se haya tecleado este fotograma.
 *
 * GetCharPressed va sacando de una cola las letras que el jugador alcanzo a
 * teclear, por eso se llama en un ciclo: si en un fotograma escribio dos, se
 * atienden las dos. Devuelve 0 cuando ya no queda ninguna.
 */
static void capturarNombre(char* nombre)
{
    int largo = (int)strlen(nombre);

    int letra = GetCharPressed();

    while(letra > 0){

        // Solo ASCII imprimible: la fuente que trae raylib de fabrica no sabe
        // dibujar acentos ni enes, y saldrian como cuadritos.
        if(letra >= 32 && letra <= 126 && largo < NOMBRE_MAX){
            nombre[largo] = (char)letra;
            largo++;
            nombre[largo] = '\0';
        }

        letra = GetCharPressed();
    }

    // IsKeyPressedRepeat es lo que hace que dejar apretado el borrador siga
    // borrando, como en cualquier campo de texto.
    if((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && largo > 0){
        largo--;
        nombre[largo] = '\0';
    }
}

/**
 * \brief Dibuja el campo de texto con el nombre y su cursor parpadeante.
 */
static void dibujarCampoNombre(const char* nombre)
{
    Rectangle campo = campoNombre();

    DrawRectangleRounded(campo, 0.25f, 8, COLOR_BOTON);
    DrawRectangleRoundedLinesEx(campo, 0.25f, 8, 2.0f, COLOR_SELECCION);

    const int TAMANO = 28;

    int x = (int)(campo.x + 16.0f);
    int y = (int)(campo.y + (campo.height - TAMANO) / 2.0f);

    DrawText(nombre, x, y, TAMANO, COLOR_TEXTO);

    // El cursor solo se dibuja media parpadeada de cada una. GetTime da los
    // segundos desde que arranco el juego; multiplicar por 2 y quedarse con la
    // paridad da dos parpadeos por segundo.
    bool visible = (((int)(GetTime() * 2.0)) % 2 == 0);

    if(visible && (int)strlen(nombre) < NOMBRE_MAX){
        DrawText("_", x + MeasureText(nombre, TAMANO) + 3, y, TAMANO, COLOR_SELECCION);
    }

    // El contador de letras avisa que hay un limite ANTES de que el jugador se
    // estrelle contra el.
    const char* contador = TextFormat("%d/%d", (int)strlen(nombre), NOMBRE_MAX);
    int ancho = MeasureText(contador, 18);

    DrawText(contador,
             (int)(campo.x + campo.width - ancho - 14.0f),
             (int)(campo.y + campo.height / 2.0f - 9.0f),
             18, COLOR_TENUE);
}

//***********************************************
// VISTA PREVIA DE LOS POZOS
//***********************************************

/**
 * \brief Dibuja, en chiquito, como van a quedar acomodados los pozos.
 *
 * Es la parte del boceto donde se ven los 5, 7 o 10 hoyos segun el nivel. No
 * reutiliza VistaTablero porque alla las medidas son las de la partida de
 * verdad; aqui hacen falta unas propias, mas chicas.
 */
static void dibujarVistaPrevia(const ReglasDificultad& reglas)
{
    Rectangle caja = cajaVistaPrevia();

    const float ANCHO   = 60.0f;
    const float ALTO    = 26.0f;
    const float SEPARA_X = 22.0f;
    const float SEPARA_Y = 50.0f;

    float centroY = caja.y + caja.height / 2.0f + 10.0f;
    float y0 = centroY - (reglas.filas - 1) * SEPARA_Y / 2.0f;

    int indice = 0;

    for(int fila = 0; fila < reglas.filas; fila++){

        int enEstaFila = reglas.pozosPorFila[fila];

        float anchoFila = enEstaFila * ANCHO + (enEstaFila - 1) * SEPARA_X;
        float x0 = caja.x + (caja.width - anchoFila) / 2.0f;

        for(int i = 0; i < enEstaFila; i++){

            float centroX = x0 + i * (ANCHO + SEPARA_X) + ANCHO / 2.0f;
            float y       = y0 + fila * SEPARA_Y;

            DrawEllipse((int)centroX, (int)y, ANCHO / 2.0f, ALTO / 2.0f, COLOR_TIERRA);
            DrawEllipse((int)centroX, (int)(y + 2.0f), ANCHO / 2.0f - 5.0f, ALTO / 2.0f - 5.0f, COLOR_POZO);

            indice++;
        }
    }

    const char* resumen = TextFormat("%d pozos     %d vida(s)     %.1f s visibles al inicio",
                                     indice, reglas.vidas, reglas.visibleInicial);

    dibujarTextoCentrado(resumen, (int)(caja.y - 4.0f), 20, COLOR_TENUE);
}

//***********************************************
// PANTALLA DE CONFIGURACION
//***********************************************

Escena_Estado ActualizarConfiguracion(ConfigPartida& config)
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;

    // El campo de nombre esta siempre activo: en esta pantalla el teclado es
    // para escribir y el mouse para elegir. Por eso aqui NO se usa confirmado()
    // como en el menu -Espacio tiene que poder escribirse dentro del nombre-.
    capturarNombre(config.nombre);

    for(int i = 0; i < NUM_DIFICULTADES; i++){
        if(botonClicado(botonDificultad(i))){
            config.dificultad = (Dificultad)i;
        }
    }

    bool listo = nombreValido(config.nombre);

    if(listo && (botonClicado(botonIniciar()) || IsKeyPressed(KEY_ENTER))){
        return Escena_juego;
    }

    return Escena_configuracion;
}

void DibujarConfiguracion(const ConfigPartida& config)
{
    dibujarTextoCentrado("CONFIGURACION DE PARTIDA", 84, 40, COLOR_TITULO);

    DrawText("Nombre:", (int)ETIQUETA_X, (int)(FILA_NOMBRE + 14.0f), 26, COLOR_TEXTO);
    dibujarCampoNombre(config.nombre);

    DrawText("Nivel:", (int)ETIQUETA_X, (int)(FILA_NIVEL + 14.0f), 26, COLOR_TEXTO);

    for(int i = 0; i < NUM_DIFICULTADES; i++){
        bool elegida = ((int)config.dificultad == i);
        dibujarBoton(botonDificultad(i), NOMBRES_DIFICULTAD[i], elegida);
    }

    dibujarVistaPrevia(reglasDe(config.dificultad));

    // El boton de iniciar se ve apagado mientras no haya nombre, en vez de
    // desaparecer: asi el jugador ve que existe y entiende que le falta algo.
    bool listo = nombreValido(config.nombre);

    if(listo){
        dibujarBoton(botonIniciar(), "Inicio", false);
    } else {
        DrawRectangleRounded(botonIniciar(), 0.25f, 8, COLOR_BOTON);

        Rectangle boton = botonIniciar();
        int ancho = MeasureText("Inicio", 25);

        DrawText("Inicio",
                 (int)(boton.x + (boton.width - ancho) / 2.0f),
                 (int)(boton.y + (boton.height - 25) / 2.0f),
                 25, COLOR_TENUE);

        dibujarTextoCentrado("Escribe tu nombre para poder empezar", 620, 20, COLOR_TENUE);
    }

    dibujarTextoCentrado("Escribe tu nombre     Clic para elegir el nivel     Enter para empezar", 664, 18, COLOR_TENUE);
    dibujarTextoCentrado("ESC para volver al menu", 690, 18, COLOR_TENUE);
}
