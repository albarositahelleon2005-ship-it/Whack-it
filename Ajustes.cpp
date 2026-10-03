/**
 * \file Ajustes.cpp
 * \brief Implementación de la ventana de ajustes.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Ajustes.hpp"
#include "Audio.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

//***********************************************
// ACOMODO DEL PANEL
//***********************************************

// El panel mide lo mismo que fondo_ajustes.png ya achicado (ver Iconos.hpp),
// para que la imagen no se deforme.
static const float PANEL_ANCHO = (float)ANCHO_PANEL_AJUSTES;
static const float PANEL_ALTO  = (float)ALTO_PANEL_AJUSTES;

// El margen es generoso porque el fondo trae flores y hojas en las orillas:
// los botones van por dentro de ese marco.
static const float MARGEN     = 80.0f;
static const float LADO_BOTON = (float)LADO_BOTON_VOLUMEN;

// A que altura, ya dentro del panel, empieza cada renglon de volumen. El
// primero va debajo del letrero de AJUSTES que trae el fondo.
static const float FILA_MUSICA  = 150.0f;
static const float FILA_EFECTOS = 250.0f;

static bool abierta = false;

// Las filas que se pueden resaltar con las flechas, de arriba a abajo. En las
// de volumen, izquierda/derecha bajan y suben; en Cerrar, Enter cierra.
enum FilaAjustes {
    FilaAj_musica,
    FilaAj_efectos,
    FilaAj_cerrar,
    NUM_FILAS_AJUSTES
};

static int filaResaltada = FilaAj_musica;
/**
 * \brief El rectangulo del panel, centrado en la ventana.
 *
 * Actualizar y dibujar lo calculan cada quien por su lado en vez de guardarlo:
 * es una resta, y sale mas barato que arriesgarse a que un boton se dibuje en
 * un lugar y se detecte el clic en otro.
 */
static Rectangle panelAjustes()
{
    return rectangulo((GetScreenWidth()  - PANEL_ANCHO) / 2.0f,
                      (GetScreenHeight() - PANEL_ALTO ) / 2.0f,
                      PANEL_ANCHO, PANEL_ALTO);
}

/// El boton de bajar volumen de un renglon.
static Rectangle botonMenos(float fila)
{
    Rectangle panel = panelAjustes();
    return rectangulo(panel.x + MARGEN, panel.y + fila, LADO_BOTON, LADO_BOTON);
}

/// El boton de subir volumen de un renglon.
static Rectangle botonMas(float fila)
{
    Rectangle panel = panelAjustes();
    return rectangulo(panel.x + panel.width - MARGEN - LADO_BOTON,
                      panel.y + fila, LADO_BOTON, LADO_BOTON);
}

/// La barra que muestra en cuanto va el volumen, entre los dos botones.
static Rectangle barraVolumen(float fila)
{
    Rectangle menos = botonMenos(fila);
    Rectangle mas   = botonMas(fila);

    const float HUECO = 14.0f;
    float inicio = menos.x + menos.width + HUECO;

    return rectangulo(inicio, menos.y + 12.0f, mas.x - HUECO - inicio, LADO_BOTON - 24.0f);
}

/// El boton de cerrar la ventana.
static Rectangle botonCerrar()
{
    Rectangle panel = panelAjustes();

    return rectangulo(panel.x + (panel.width - ANCHO_BOTON_CERRAR) / 2.0f,
                      panel.y + panel.height - 92.0f,
                      (float)ANCHO_BOTON_CERRAR, (float)ALTO_BOTON_CERRAR);
}

/**
 * \brief El area que cuenta como "encima" de cada fila, para que el mouse la
 * resalte: de la etiqueta al boton "+", o el boton de cerrar.
 */
static Rectangle areaFila(int fila)
{
    if(fila == FilaAj_cerrar) return botonCerrar();

    float     y     = (fila == FilaAj_musica) ? FILA_MUSICA : FILA_EFECTOS;
    Rectangle menos = botonMenos(y);
    Rectangle mas   = botonMas(y);

    return rectangulo(menos.x, menos.y - 34.0f, mas.x + mas.width - menos.x, mas.height + 34.0f);
}

/**
 * \brief Dibuja un boton de volumen ("+" o "-").
 *
 * Esas imagenes no traen version prendida, asi que el mouse encima se marca
 * agrandandolas un poco: se nota sin necesitar otro dibujo.
 */
static void dibujarBotonVolumen(Rectangle rec, Texture2D textura, const char* repuesto)
{
    if(textura.id != 0 && ratonEncima(rec)){
        const float CRECE = 4.0f;
        rec = rectangulo(rec.x - CRECE, rec.y - CRECE, rec.width + CRECE * 2.0f, rec.height + CRECE * 2.0f);
    }

    dibujarBotonImagen(rec, textura, repuesto, false);
}

/**
 * \brief Dibuja un renglon completo: etiqueta, boton de bajar, barra y boton
 * de subir.
 */
static void dibujarRenglon(const char* etiqueta, float fila, float nivel, bool resaltado)
{
    Rectangle panel = panelAjustes();

    // La fila resaltada lleva el ">" al frente y la etiqueta en otro color,
    // como en el menu y en la configuracion.
    Color colorEtiqueta = resaltado ? COLOR_SELECCION : COLOR_TEXTO;
    int   xEtiqueta     = (int)(panel.x + MARGEN);
    int   yEtiqueta     = (int)(panel.y + fila - 30.0f);

    if(resaltado){
        dibujarTexto(">", xEtiqueta - 24, yEtiqueta, 22, COLOR_SELECCION);
    }

    dibujarTexto(etiqueta, xEtiqueta, yEtiqueta, 22, colorEtiqueta);

    dibujarBotonVolumen(botonMenos(fila), botonVolumenMenos(), "-");
    dibujarBotonVolumen(botonMas(fila),   botonVolumenMas(),   "+");

    Rectangle barra = barraVolumen(fila);

    DrawRectangleRounded(barra, 0.5f, 8, ColorAlpha(COLOR_TEXTO_MADERA, 0.35f));

    // La parte llena es la misma barra pero con el ancho recortado al nivel.
    // Con volumen en cero no se dibuja nada: un rectangulo de ancho cero se
    // vería como una rayita suelta por el redondeo.
    if(nivel > 0.0f){
        Rectangle llena = barra;
        llena.width = barra.width * nivel;
        DrawRectangleRounded(llena, 0.5f, 8, COLOR_BOTON);
    }

    DrawRectangleRoundedLinesEx(barra, 0.5f, 8, 2.0f, COLOR_TEXTO_MADERA);

    // El porcentaje va a la derecha de la barra, en numero, porque de un
    // vistazo la barra dice "como va" pero no dice "cuanto es".
    const char* texto = TextFormat("%d%%", (int)(nivel * 100.0f + 0.5f));
    int ancho = medirTexto(texto, 20);

    dibujarTexto(texto,
             (int)(barra.x + barra.width - ancho),
             (int)(panel.y + fila - 28.0f),
             20, COLOR_TEXTO);
}

//***********************************************
// VENTANA DE AJUSTES
//***********************************************

void abrirAjustes()
{
    abierta       = true;
    filaResaltada = FilaAj_musica;
}

bool ajustesAbiertos()
{
    return abierta;
}

void ActualizarAjustes()
{
    if(!abierta) return;

    if(IsKeyPressed(KEY_ESCAPE)){
        abierta = false;
        return;
    }

    if(botonClicado(botonMenos(FILA_MUSICA)))  ajustarVolumenMusica(-PASO_VOLUMEN);
    if(botonClicado(botonMas(FILA_MUSICA)))    ajustarVolumenMusica( PASO_VOLUMEN);

    if(botonClicado(botonMenos(FILA_EFECTOS))) ajustarVolumenEfectos(-PASO_VOLUMEN);
    if(botonClicado(botonMas(FILA_EFECTOS)))   ajustarVolumenEfectos( PASO_VOLUMEN);

    if(botonClicado(botonCerrar())) abierta = false;

    // Teclado: las flechas y el mouse mueven el mismo resaltado, como en el menu.
    Rectangle areas[NUM_FILAS_AJUSTES];
    for(int i = 0; i < NUM_FILAS_AJUSTES; i++) areas[i] = areaFila(i);

    moverSeleccion(filaResaltada, NUM_FILAS_AJUSTES, KEY_DOWN, KEY_UP);
    seguirRaton(areas, NUM_FILAS_AJUSTES, filaResaltada);

    float cambio = 0.0f;
    if(IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) cambio += PASO_VOLUMEN;
    if(IsKeyPressed(KEY_LEFT)  || IsKeyPressedRepeat(KEY_LEFT))  cambio -= PASO_VOLUMEN;

    if(filaResaltada == FilaAj_musica  && cambio != 0.0f) ajustarVolumenMusica(cambio);
    if(filaResaltada == FilaAj_efectos && cambio != 0.0f) ajustarVolumenEfectos(cambio);

    // Enter solo hace algo sobre Cerrar: en las filas de volumen no hay nada
    // que "elegir", se ajustan con izquierda y derecha.
    bool enter = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
    if(filaResaltada == FilaAj_cerrar && enter) abierta = false;
}

void DibujarAjustes()
{
    if(!abierta) return;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), COLOR_VELO);

    Rectangle panel = panelAjustes();
    Texture2D fondo = fondoAjustes();

    // El fondo ya trae el letrero de AJUSTES. Si falta, se dibuja el panel
    // liso de antes con su titulo escrito, para que la ventana se entienda.
    if(fondo.id != 0){
        DrawTexture(fondo, (int)panel.x, (int)panel.y, WHITE);
    } else {
        DrawRectangleRounded(panel, 0.08f, 10, COLOR_FONDO);
        DrawRectangleRoundedLinesEx(panel, 0.08f, 10, 2.0f, COLOR_SELECCION);

        const char* titulo = "AJUSTES";
        int ancho = medirTexto(titulo, 34);

        dibujarTexto(titulo,
                 (int)(panel.x + (panel.width - ancho) / 2.0f),
                 (int)(panel.y + 32.0f),
                 34, COLOR_TITULO);
    }

    dibujarRenglon("Musica",        FILA_MUSICA,  volumenMusica(),  filaResaltada == FilaAj_musica);
    dibujarRenglon("Sonidos (SFX)", FILA_EFECTOS, volumenEfectos(), filaResaltada == FilaAj_efectos);

    // Cerrar se prende (_P) cuando es la fila resaltada, por flechas o mouse.
    bool cerrarResaltado = (filaResaltada == FilaAj_cerrar);
    dibujarBotonImagen(botonCerrar(), botonCerrarImg(cerrarResaltado), "Cerrar", cerrarResaltado);
}
