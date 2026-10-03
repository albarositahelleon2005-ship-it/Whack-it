/**
 * \file Juego.cpp
 * \brief Implementación de la pantalla de juego.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Juego.hpp"
#include "Partida.hpp"
#include "VistaTablero.hpp"
#include "Pausa.hpp"
#include "Puntaje.hpp"
#include "Resultados.hpp"
#include "Audio.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

// 'static' a nivel de archivo: nadie fuera de aqui necesita ver la partida en
// curso. Es el mismo criterio que opcionSeleccionada en Menu.cpp.
static Partida partida;
static bool    enPausa = false;

// Se guarda la configuracion con la que se arranco para poder reiniciar desde
// la pausa sin tener que volver a pasar por la pantalla de configuracion.
static ConfigPartida configActual;

void IniciarPartida(const ConfigPartida& config)
{
    configActual = config;

    iniciarPartida(partida, config);
    enPausa = false;
}

void AbrirPausa()
{
    if(partida.terminada) return;
    enPausa = true;
    prepararPausa();
}

Escena_Estado ActualizarJuego()
{
    // 1. Si ya se acabo, lo unico vivo es la ventana de FIN.
    if(partida.terminada){
        if(ActualizarResultados() == Resultados_menu) return Escena_menu;
        return Escena_juego;
    }

    // 2. Si esta en pausa, el reloj de la partida ni se toca: no llamar a
    //    avanzarPartida es, literalmente, lo que la congela.
    if(enPausa){
        AccionPausa accion = ActualizarPausa();

        switch(accion)
        {
            case Pausa_continuar:
                enPausa = false;
            break;

            case Pausa_reiniciar:
                IniciarPartida(configActual);
            break;

            case Pausa_menu:
                enPausa = false;
                return Escena_menu;

            case Pausa_ninguna:
            break;
        }

        return Escena_juego;
    }

    if(IsKeyPressed(KEY_ESCAPE)){
        AbrirPausa();
        return Escena_juego;
    }

    // 3. El golpe va ANTES de avanzar el reloj, no despues: asi un topo al que
    //    le quedaba una milesima de segundo todavia cuenta como golpeado. Es
    //    la diferencia entre sentirse justo e injusto.
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

        int indice = pozoEn(partida, GetMousePosition());

        switch(golpearPozo(partida, indice))
        {
            case Golpe_enemigo:
            case Golpe_premium: sonarGolpe(); break;
            case Golpe_bomba:   sonarBomba(); break;
            case Golpe_aire:    break;
        }
    }

    avanzarPartida(partida, GetFrameTime());

    // 4. Si ese ultimo golpe acabo con las vidas, se congelan los numeros para
    //    el panel de FIN, que ya se dibuja este mismo fotograma.
    //    Y se anota en la tabla de puntajes. Esto corre una sola vez por
    //    partida: desde el siguiente fotograma, el paso 1 regresa antes.
    if(partida.terminada){
        prepararResultados(partida.nombre, partida.puntaje, partida.mejorCombo);
        anotarPuntaje(configActual.dificultad, partida.nombre, partida.puntaje, partida.mejorCombo);
    }

    return Escena_juego;
}

void DibujarJuego()
{
    // El fondo de la partida ya trae el letrero de WHACK IT!; el texto solo
    // hace falta si la imagen no cargo.
    dibujarFondo(fondoJuego());

    if(fondoJuego().id == 0) dibujarTextoCentrado("WHACK IT!", 20, 34, COLOR_TITULO);

    dibujarTablero(partida);
    dibujarMarcador(partida);

    // Las ventanas virtuales van AL FINAL, despues de la partida, para que
    // queden encima -el mismo orden que usaba Gatorama con el tablero-.
    if(partida.terminada){
        DibujarResultados();
    } else if(enPausa){
        DibujarPausa();
    }
}
