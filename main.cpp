/**
 * \file main.cpp
 * \brief Archivo principal de Whack it.
 * \date 13/09/2026
 */

#include "raylib.h"
#include <cstdlib>
#include <ctime>

#include "Menu.hpp"
#include "Dibujo.hpp"
#include "Tema.hpp"
#include "ConfigPartida.hpp"
#include "Configuracion.hpp"
#include "Puntaje.hpp"
#include "Instrucciones.hpp"
#include "Opciones.hpp"
#include "Creditos.hpp"
#include "Juego.hpp"
#include "Iconos.hpp"
#include "Sprites.hpp"
#include "BarraSuperior.hpp"
#include "Ajustes.hpp"
#include "Audio.hpp"

// ***********************************************
// CONFIGURACION DE LA VENTANA
// ***********************************************

const int PantallaAncho = 1280;
const int PantallaAlto  = 720;
const int FPS = 60;

// ***********************************************

int main()
{
    // La semilla es la que hace que los topos no salgan siempre en el mismo
    // orden: sin ella, aleatorio() daria la misma secuencia cada partida.
    srand(time(NULL));

    // Un .exe se puede abrir desde donde sea: un acceso directo del escritorio,
    // el menu inicio, una USB. En esos casos Windows NO deja la carpeta del
    // juego como carpeta de trabajo, y todas las rutas tipo "recursos/..." se
    // buscarian en el lugar equivocado: el juego abriria sin imagenes ni
    // musica. Esta linea planta la carpeta de trabajo donde esta el .exe, y
    // con eso el juego funciona igual sin importar como lo hayan abierto.
    ChangeDirectory(GetApplicationDirectory());

    InitWindow(PantallaAncho, PantallaAlto, "Whack it");
    SetTargetFPS(FPS);

    // Por defecto WindowShouldClose() tambien es verdadero al presionar ESC,
    // no solo al cerrar la ventana. Como ESC se usa para regresar de pantalla
    // en pantalla, hay que quitarle ese trabajo. Sin esta linea, ESC cierra
    // el juego desde cualquier lado.
    SetExitKey(KEY_NULL);

    // Preparar los gifs se tarda un par de segundos (ver Animacion.hpp). Sin
    // avisar nada, la ventana se queda en blanco y Windows hasta puede marcarla
    // como "no responde". Pintar un fotograma ANTES de cargar cuesta tres
    // lineas y cambia por completo la primera impresion del juego.
    BeginDrawing();
        ClearBackground(COLOR_FONDO);
        dibujarTextoCentrado("Cargando...", PantallaAlto / 2 - 20, 32, COLOR_TITULO);
    EndDrawing();

    // Cargar imagenes, gifs y musica requiere que la ventana ya este abierta,
    // por eso van despues de InitWindow y no antes.
    CargarIconos();
    CargarSprites();
    IniciarAudio();

    Escena_Estado escenaActual = Escena_menu;

    // Lo que el jugador elige en la pantalla de configuracion. Vive aqui,
    // en el bucle, porque es lo unico que dos pantallas distintas se tienen
    // que pasar: la configuracion lo llena y el juego lo lee.
    ConfigPartida config = configPorDefecto();

    // Loop principal del juego.
    // Se sale por la X de la ventana o cuando el menu pide Escena_salir.
    while(!WindowShouldClose() && escenaActual != Escena_salir){

        // La musica necesita que se le de tiempo cada fotograma para seguir
        // sonando (ver Audio.hpp). Va fuera de todo lo demas porque debe
        // seguir sonando aunque el juego este en pausa o en ajustes.
        ActualizarAudio();

        // Que va en la esquina de arriba a la izquierda depende de donde
        // estemos: en el menu no hay a donde volver, y en la partida ese lugar
        // lo ocupa el boton de pausa en vez de la flecha de regresar.
        IconoIzquierdo iconoIzq = Izq_regresar;

        if(escenaActual == Escena_menu)       iconoIzq = Izq_ninguno;
        else if(escenaActual == Escena_juego) iconoIzq = Izq_pausa;

        // -------------------------------------------
        // ACTUALIZAR: leer entrada y cambiar el estado
        // -------------------------------------------

        if(ajustesAbiertos()){

            // Con los ajustes abiertos, la pantalla de abajo no recibe entrada
            // NI avanza su reloj: no llamar a su Actualizar es justamente lo
            // que congela la partida mientras se cambia el volumen.
            ActualizarAjustes();

        } else {

            // Los iconos de la barra se revisan ANTES que la pantalla, y si
            // alguno se uso, la pantalla ya no ve ese clic. Sin esto, picarle
            // al engrane durante la partida contaria ademas como un golpe.
            bool consumido = false;

            if(iconoAjustesClicado()){
                abrirAjustes();
                consumido = true;
            } else if(iconoIzq != Izq_ninguno && iconoIzquierdoClicado()){
                if(iconoIzq == Izq_pausa) AbrirPausa();
                else                      escenaActual = Escena_menu;
                consumido = true;
            }

            if(!consumido){
                switch(escenaActual)
                {
                    case Escena_menu:
                        escenaActual = ActualizarMenu();
                    break;

                    case Escena_configuracion:
                    {
                        Escena_Estado siguiente = ActualizarConfiguracion(config);

                        // La configuracion solo dice "ya quedo". Quien arma la
                        // partida es el juego, y por eso se arma justo aqui, en
                        // el brinco entre las dos pantallas.
                        if(siguiente == Escena_juego) IniciarPartida(config);

                        escenaActual = siguiente;
                    }
                    break;

                    case Escena_puntajes:
                        escenaActual = ActualizarPuntaje();
                    break;

                    case Escena_instrucciones:
                        escenaActual = ActualizarInstrucciones();
                    break;

                    case Escena_opciones:
                        escenaActual = ActualizarOpciones();
                    break;

                    case Escena_creditos:
                        escenaActual = ActualizarCreditos();
                    break;

                    case Escena_juego:
                        escenaActual = ActualizarJuego();
                    break;

                    default: break;
                }
            }
        }

        // -------------------------------------------
        // DIBUJAR: pintar el estado, sin modificarlo
        // -------------------------------------------
        BeginDrawing();
            ClearBackground(COLOR_FONDO);

            switch(escenaActual)
            {
                case Escena_menu:
                    DibujarMenu();
                break;

                case Escena_configuracion:
                    DibujarConfiguracion(config);
                break;

                case Escena_puntajes:
                    DibujarPuntaje();
                break;

                case Escena_instrucciones:
                    DibujarInstrucciones();
                break;

                case Escena_opciones:
                    DibujarOpciones();
                break;

                case Escena_creditos:
                    DibujarCreditos();
                break;

                case Escena_juego:
                    DibujarJuego();
                break;

                default: break;
            }

            // Encima de la pantalla, igual que la pausa se dibuja encima del
            // juego. El engrane aparece en todas, incluida la partida.
            dibujarBarraSuperior(iconoIzq);

            // Y los ajustes encima de absolutamente todo, porque se pueden
            // abrir desde cualquier lado.
            DibujarAjustes();

        EndDrawing();
    }

    TerminarAudio();
    DescargarSprites();
    DescargarIconos();
    CloseWindow();

    return 0;
}
