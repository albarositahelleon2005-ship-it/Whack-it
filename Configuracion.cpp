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
#include "Iconos.hpp"
#include "Sprites.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DE LA PANTALLA
//***********************************************

static const float ETIQUETA_X = 200.0f;
static const float CAMPO_X    = 360.0f;

static const float FILA_NOMBRE = 178.0f;
static const float FILA_NIVEL   = 262.0f;

// Los tamanos los fija Iconos.hpp, porque es ahi donde se achican las
// imagenes de madera a la medida exacta en que se dibujan.
static const float BOTON_NIVEL_ANCHO = (float)ANCHO_BOTON_NIVEL;
static const float BOTON_NIVEL_ALTO  = (float)ALTO_BOTON_NIVEL;
static const float SEPARA_NIVEL      =  20.0f;

/// El rectángulo donde se escribe el nombre.
static Rectangle campoNombre()
{
    return rectangulo(CAMPO_X, FILA_NOMBRE, (float)ANCHO_MARCO_NOMBRE, (float)ALTO_MARCO_NOMBRE);
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
    // Separado de la esquina derecha unos 2 cm (75 px en un monitor comun)
    // mas de lo que estaba, a peticion del equipo.
    return rectangulo(GetScreenWidth() - 335.0f, 600.0f,
                      (float)ANCHO_BOTON_INICIO, (float)ALTO_BOTON_INICIO);
}

/// La caja donde se muestra cómo van a quedar los pozos.
static Rectangle cajaVistaPrevia()
{
    // Es el parche de tierra: centrado en la ventana, debajo de los niveles.
    return rectangulo((GetScreenWidth() - ANCHO_TIERRA_CONFIG) / 2.0f, 375.0f,
                      (float)ANCHO_TIERRA_CONFIG, (float)ALTO_TIERRA_CONFIG);
}

//***********************************************
// FILA RESALTADA
//***********************************************

// Igual que en el menu principal, hay una fila resaltada que siguen tanto las
// flechas como el mouse. Aqui las filas son tres: nombre, nivel e inicio.
enum FilaConfig {
    Fila_nombre,
    Fila_nivel,
    Fila_iniciar,
    NUM_FILAS_CONFIG
};

static int filaResaltada = Fila_nombre;

/// El área que cuenta como "encima" de cada fila, para que el mouse la resalte.
static Rectangle areaFila(int fila)
{
    switch(fila)
    {
        case Fila_nombre:
            return rectangulo(ETIQUETA_X, FILA_NOMBRE, campoNombre().x + campoNombre().width - ETIQUETA_X, campoNombre().height);

        case Fila_nivel:
        {
            Rectangle ultimo = botonDificultad(NUM_DIFICULTADES - 1);
            return rectangulo(ETIQUETA_X, FILA_NIVEL, ultimo.x + ultimo.width - ETIQUETA_X, BOTON_NIVEL_ALTO);
        }

        default:
            return botonIniciar();
    }
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
    Texture2D marco = marcoNombre();

    // Detras del nombre va la tabla de madera. Si la imagen falta, se cae al
    // rectangulo de siempre para que el campo se siga viendo.
    Color tinta = COLOR_TEXTO_MADERA;

    if(marco.id != 0){
        DrawTexture(marco, (int)campo.x, (int)campo.y, WHITE);
    } else {
        DrawRectangleRounded(campo, 0.25f, 8, COLOR_BOTON);
        DrawRectangleRoundedLinesEx(campo, 0.25f, 8, 2.0f, COLOR_SELECCION);
        tinta = COLOR_TEXTO;
    }

    const int TAMANO = 28;

    int x = (int)(campo.x + 22.0f);
    int y = (int)(campo.y + (campo.height - TAMANO) / 2.0f);

    dibujarTexto(nombre, x, y, TAMANO, tinta);

    // El cursor solo se dibuja media parpadeada de cada una. GetTime da los
    // segundos desde que arranco el juego; multiplicar por 2 y quedarse con la
    // paridad da dos parpadeos por segundo.
    bool visible = (((int)(GetTime() * 2.0)) % 2 == 0);

    if(visible && (int)strlen(nombre) < NOMBRE_MAX){
        dibujarTexto("_", x + medirTexto(nombre, TAMANO) + 3, y, TAMANO, tinta);
    }

    // El contador de letras avisa que hay un limite ANTES de que el jugador se
    // estrelle contra el.
    const char* contador = TextFormat("%d/%d", (int)strlen(nombre), NOMBRE_MAX);
    int ancho = medirTexto(contador, 18);

    dibujarTexto(contador,
             (int)(campo.x + campo.width - ancho - 22.0f),
             (int)(campo.y + campo.height / 2.0f - 9.0f),
             18, ColorAlpha(tinta, 0.6f));
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
static void dibujarVistaPrevia(Dificultad nivel)
{
    ReglasDificultad reglas = reglasDe(nivel);

    // En que pozo de la vista previa se asoma un mapache, por nivel (contando
    // desde cero, en el orden de los pozos: de arriba a abajo y de izquierda
    // a derecha). Es puro adorno: dice "de aqui salen" sin tener que leerlo.
    static const int POZO_MAPACHE[NUM_DIFICULTADES] = {
        3,   // facil:   el 4, primero de la fila de abajo
        6,   // normal:  el 7, ultimo de la fila de abajo
        8    // dificil: el 9, el de en medio de la fila de abajo
    };

    Rectangle caja   = cajaVistaPrevia();
    Texture2D tierra = fondoTierraConfig();

    if(tierra.id != 0) DrawTexture(tierra, (int)caja.x, (int)caja.y, WHITE);

    // Misma proporcion que hoyo.png (ANCHO_HOYO x ALTO_HOYO), para no deformarlo.
    const float ANCHO   = 60.0f;
    const float ALTO    = ANCHO * ALTO_HOYO / ANCHO_HOYO;

    Texture2D hoyo = imagenHoyo();
    // Con 4 pozos en una fila (normal y dificil) ocupan 294 px: caben por
    // dentro de la orilla ondulada del parche de tierra.
    const float SEPARA_X = 18.0f;
    const float SEPARA_Y = 50.0f;

    float centroY = caja.y + caja.height / 2.0f;
    float y0 = centroY - (reglas.filas - 1) * SEPARA_Y / 2.0f;

    int indice = 0;

    for(int fila = 0; fila < reglas.filas; fila++){

        int enEstaFila = reglas.pozosPorFila[fila];

        float anchoFila = enEstaFila * ANCHO + (enEstaFila - 1) * SEPARA_X;
        float x0 = caja.x + (caja.width - anchoFila) / 2.0f;

        for(int i = 0; i < enEstaFila; i++){

            float centroX = x0 + i * (ANCHO + SEPARA_X) + ANCHO / 2.0f;
            float y       = y0 + fila * SEPARA_Y;

            // El mapache va ANTES que su hoyo, para que el monticulo le tape
            // los pies, igual que en la partida (ver dibujarTablero). Mismas
            // proporciones que alla: tan ancho como el hoyo y con la base al
            // 70 % del alto del hoyo.
            if(indice == POZO_MAPACHE[nivel] && animacionLista(spriteEnemigo())){
                float base = (y - ALTO / 2.0f) + ALTO * 0.7f;
                Rectangle destino = { centroX - ANCHO / 2.0f, base - ANCHO, ANCHO, ANCHO };
                dibujarAnimacion(spriteEnemigo(), destino, 0.0f);
            }

            // El mismo hoyo.png de la partida, en chiquito. Si falta, las
            // elipses de siempre.
            if(hoyo.id != 0){
                Rectangle origen  = { 0.0f, 0.0f, (float)hoyo.width, (float)hoyo.height };
                Rectangle destino = { centroX - ANCHO / 2.0f, y - ALTO / 2.0f, ANCHO, ALTO };
                DrawTexturePro(hoyo, origen, destino, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
            } else {
                DrawEllipse((int)centroX, (int)y, ANCHO / 2.0f, ALTO / 2.0f, COLOR_TIERRA);
                DrawEllipse((int)centroX, (int)(y + 2.0f), ANCHO / 2.0f - 5.0f, ALTO / 2.0f - 5.0f, COLOR_POZO);
            }

            indice++;
        }
    }

    const char* resumen = TextFormat("%d pozos   %d vida(s)   %.1f s visibles al inicio",
                                     indice, reglas.vidas, reglas.visibleInicial);

    // Encima de la tierra y no adentro: el parche es mas angosto que el
    // resumen. Va sobre la madera oscura del fondo, en el crema claro, igual
    // que las etiquetas (ver DibujarConfiguracion).
    Color claro = (fondoConfiguracion().id != 0) ? COLOR_TEXTO_CLARO : COLOR_TEXTO;
    dibujarTextoCentrado(resumen, (int)(caja.y - 30.0f), 20, claro);
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

    // Las flechas no chocan con el campo de nombre: GetCharPressed solo
    // devuelve caracteres imprimibles, nunca flechas.
    Rectangle areas[NUM_FILAS_CONFIG];
    for(int i = 0; i < NUM_FILAS_CONFIG; i++) areas[i] = areaFila(i);

    moverSeleccion(filaResaltada, NUM_FILAS_CONFIG, KEY_DOWN, KEY_UP);
    seguirRaton(areas, NUM_FILAS_CONFIG, filaResaltada);

    if(filaResaltada == Fila_nivel){
        int nivel = (int)config.dificultad;

        if(IsKeyPressed(KEY_RIGHT) && nivel < NUM_DIFICULTADES - 1) nivel++;
        if(IsKeyPressed(KEY_LEFT)  && nivel > 0)                    nivel--;

        config.dificultad = (Dificultad)nivel;
    }

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
    dibujarFondo(fondoConfiguracion());

    // El fondo de esta pantalla es madera oscura: el cafe #552000 del resto
    // del juego se perderia, asi que lo que va directo sobre el fondo usa el
    // crema claro. Lo que va sobre madera clara (nombre, botones) sigue en cafe.
    Color claro = (fondoConfiguracion().id != 0) ? COLOR_TEXTO_CLARO : COLOR_TEXTO;

    // Mas abajo que el resto de los titulos para que quede centrado en la
    // tabla oscura que el fondo trae dibujada arriba.
    dibujarTextoCentrado("CONFIGURACION DE PARTIDA", 103, 40, claro);

    // La fila resaltada lleva el ">" al frente y la etiqueta en otro color, como
    // en el menu. El ">" va a la izquierda de la etiqueta para no recorrerla.
    Color colorNombre = (filaResaltada == Fila_nombre) ? COLOR_SELECCION : claro;
    Color colorNivel  = (filaResaltada == Fila_nivel)  ? COLOR_SELECCION : claro;

    // Las etiquetas van alineadas a la derecha, pegadas a su campo: asi se
    // leen junto a lo que nombran y no sueltas sobre la pared del fondo.
    const int   TAMANO_ETIQUETA = 26;
    const float PEGADO          = 18.0f;

    float xNombre = CAMPO_X - PEGADO - medirTexto("Nombre:", TAMANO_ETIQUETA);
    float xNivel  = CAMPO_X - PEGADO - medirTexto("Nivel:",  TAMANO_ETIQUETA);

    if(filaResaltada != Fila_iniciar){
        bool  enNombre = (filaResaltada == Fila_nombre);
        float y        = enNombre ? FILA_NOMBRE : FILA_NIVEL;
        float x        = enNombre ? xNombre : xNivel;

        dibujarTexto(">", (int)(x - 30.0f), (int)(y + 14.0f), TAMANO_ETIQUETA, COLOR_SELECCION);
    }

    dibujarTexto("Nombre:", (int)xNombre, (int)(FILA_NOMBRE + 14.0f), TAMANO_ETIQUETA, colorNombre);
    dibujarCampoNombre(config.nombre);

    dibujarTexto("Nivel:", (int)xNivel, (int)(FILA_NIVEL + 14.0f), TAMANO_ETIQUETA, colorNivel);

    for(int i = 0; i < NUM_DIFICULTADES; i++){
        dibujarBotonNivel(botonDificultad(i), i, (int)config.dificultad == i);
    }

    dibujarVistaPrevia(config.dificultad);

    // El boton de iniciar se ve apagado mientras no haya nombre, en vez de
    // desaparecer: asi el jugador ve que existe y entiende que le falta algo.
    bool listo = nombreValido(config.nombre);

    // Prendido (_P) cuando es la fila resaltada, igual que los botones del
    // menu. Sin nombre se queda apagado y oscurecido.
    Texture2D inicio = botonInicio(listo && filaResaltada == Fila_iniciar);

    if(inicio.id != 0){
        Rectangle boton = botonIniciar();
        DrawTexture(inicio, (int)boton.x, (int)boton.y, listo ? WHITE : GRAY);

        if(!listo){
            dibujarTextoCentrado("Escribe tu nombre para poder empezar", 620, 20, COLOR_TEXTO_MADERA);
        }
    } else if(listo){
        dibujarBoton(botonIniciar(), "Inicio", false);

        if(filaResaltada == Fila_iniciar){
            DrawRectangleRoundedLinesEx(botonIniciar(), 0.25f, 8, 2.0f, COLOR_SELECCION);
        }
    } else {
        DrawRectangleRounded(botonIniciar(), 0.25f, 8, COLOR_BOTON);

        Rectangle boton = botonIniciar();
        int ancho = medirTexto("Inicio", 25);

        dibujarTexto("Inicio",
                 (int)(boton.x + (boton.width - ancho) / 2.0f),
                 (int)(boton.y + (boton.height - 25) / 2.0f),
                 25, COLOR_TENUE);

        dibujarTextoCentrado("Escribe tu nombre para poder empezar", 620, 20, COLOR_TEXTO_MADERA);
    }

    // Cae sobre el piso de arena del fondo: en cafe se lee, en el gris tenue
    // de las demas pantallas no.
    dibujarTextoCentrado("ESC para volver al menu", 690, 18, COLOR_TEXTO_MADERA);
}
