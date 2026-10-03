/**
 * \file Iconos.cpp
 * \brief Implementación de la carga de íconos.
 * \date 13/09/2026
 */

#include "Iconos.hpp"

// 'static' a nivel de archivo: nadie fuera de aqui toca estas texturas
// directamente, solo a traves de las funciones de abajo. Asi, si un dia cambia
// como se guardan, solo hay que tocar este archivo.
static Texture2D texturaRegresar;
static Texture2D texturaPausa;
static Texture2D texturaAjustes;

static Texture2D texturaFondoPrincipal;
static Texture2D texturaFondoJuego;
static Texture2D texturaFondoConfiguracion;
static Texture2D texturaFondoInstrucciones;
static Texture2D texturaFondoCreditos;
static Texture2D texturaFondoPuntaje;

// Botones de madera del menu principal, en el mismo orden que las opciones de
// Menu.cpp. Cada uno tiene dos versiones: A (apagado) y P (prendido).
static const int NUM_BOTONES_INTRO = 5;
static const char* NOMBRES_INTRO[NUM_BOTONES_INTRO] = {
    "jugar", "puntaje", "instrucciones", "creditos", "salir"
};
static Texture2D texturaIntroApagado[NUM_BOTONES_INTRO];
static Texture2D texturaIntroPrendido[NUM_BOTONES_INTRO];

// Botones de nivel de la configuracion, en el orden de Dificultad.hpp.
static const int NUM_BOTONES_NIVEL = 3;
static const char* NOMBRES_NIVEL[NUM_BOTONES_NIVEL] = {
    "facil", "normal", "dificil"
};
static Texture2D texturaNivelApagado[NUM_BOTONES_NIVEL];
static Texture2D texturaNivelPrendido[NUM_BOTONES_NIVEL];

static Texture2D texturaMarcoNombre;
static Texture2D texturaTierraConfig;
static Texture2D texturaInicioApagado;
static Texture2D texturaInicioPrendido;
static Texture2D texturaHoyo;

static Texture2D texturaVida;
static Texture2D texturaVidaPerdida;

// Botones de la pausa, en el orden del enum BotonPausa.
static const char* NOMBRES_PAUSA[NUM_BOTONES_PAUSA_IMG] = {
    "continuar", "reiniciarPartida", "regresarALmenu"
};
static Texture2D texturaPausaApagado[NUM_BOTONES_PAUSA_IMG];
static Texture2D texturaPausaPrendido[NUM_BOTONES_PAUSA_IMG];

static Texture2D texturaFondoAjustes;
static Texture2D texturaFondoFin;
static Texture2D texturaMapacheFin;
static Texture2D texturaVolumenMas;
static Texture2D texturaVolumenMenos;
static Texture2D texturaCerrarApagado;
static Texture2D texturaCerrarPrendido;

/**
 * \brief Carga una imagen ya achicada al tamaño en que se va a dibujar.
 *
 * Los PNG de los botones miden miles de pixeles por lado (hasta 17500 de
 * ancho): subidos tal cual pesan decenas de MB de video cada uno y rebasan el
 * tamaño máximo de textura de muchas GPU (8192), así que ni cargarían. Se
 * achican en RAM una sola vez y a la GPU solo sube la versión chica; además,
 * dibujarla a su tamaño exacto la deja nítida.
 */
static Texture2D cargarAchicada(const char* ruta, int ancho, int alto)
{
    Image imagen = LoadImage(ruta);
    if(imagen.data == nullptr) return Texture2D{};

    ImageResize(&imagen, ancho, alto);
    Texture2D textura = LoadTextureFromImage(imagen);
    UnloadImage(imagen);
    SetTextureFilter(textura, TEXTURE_FILTER_BILINEAR);
    return textura;
}

void CargarIconos()
{
    // Los tres se cargan al tamano grande, el del icono de la ventana de
    // pausa: en la barra se dibujan mas chicos y crecen con el mouse encima,
    // y achicar se ve bien; estirar, borroso.
    texturaRegresar = cargarAchicada("recursos/botones/boton_regresar.png",
                                     LADO_ICONO_PAUSA_GRANDE, LADO_ICONO_PAUSA_GRANDE);
    texturaAjustes  = cargarAchicada("recursos/botones/boton_ajustes.png",
                                     LADO_ICONO_PAUSA_GRANDE, LADO_ICONO_PAUSA_GRANDE);
    texturaPausa    = cargarAchicada("recursos/botones/boton_pausa.png",
                                     LADO_ICONO_PAUSA_GRANDE, LADO_ICONO_PAUSA_GRANDE);

    // fondo_juego.png es el boceto en blanco de antes; el fondo de verdad de
    // la partida es fondo_partida.png.
    texturaFondoPrincipal     = LoadTexture("recursos/fondo/fondo_principal.png");
    texturaFondoJuego         = LoadTexture("recursos/fondo/fondo_partida.png");
    texturaFondoConfiguracion = LoadTexture("recursos/fondo/fondo_configuracionDEpartida.png");
    texturaFondoInstrucciones = LoadTexture("recursos/fondo/fondo_instrucciones.png");
    texturaFondoCreditos      = LoadTexture("recursos/fondo/fondo_creditos.png");
    texturaFondoPuntaje       = LoadTexture("recursos/fondo/fondo_puntaje.png");

    for(int i = 0; i < NUM_BOTONES_INTRO; i++){
        texturaIntroApagado[i]  = cargarAchicada(
            TextFormat("recursos/botones/boton_intro_%s_A.png", NOMBRES_INTRO[i]),
            ANCHO_BOTON_INTRO, ALTO_BOTON_INTRO);
        texturaIntroPrendido[i] = cargarAchicada(
            TextFormat("recursos/botones/boton_intro_%s_P.png", NOMBRES_INTRO[i]),
            ANCHO_BOTON_INTRO, ALTO_BOTON_INTRO);
    }

    for(int i = 0; i < NUM_BOTONES_NIVEL; i++){
        texturaNivelApagado[i]  = cargarAchicada(
            TextFormat("recursos/botones/boton_configuracion_nivel_%s_A.png", NOMBRES_NIVEL[i]),
            ANCHO_BOTON_NIVEL, ALTO_BOTON_NIVEL);
        texturaNivelPrendido[i] = cargarAchicada(
            TextFormat("recursos/botones/boton_configuracion_nivel_%s_P.png", NOMBRES_NIVEL[i]),
            ANCHO_BOTON_NIVEL, ALTO_BOTON_NIVEL);
    }

    texturaMarcoNombre = cargarAchicada("recursos/botones/boton_configuracion_nombre.png",
                                        ANCHO_MARCO_NOMBRE, ALTO_MARCO_NOMBRE);

    // El PNG llego como lienzo de 1280x720 con fondo blanco; la copia de
    // recursos/ ya esta recortada al parche y con el blanco de afuera
    // transparente (el original quedo en arte_original/fondo/).
    texturaTierraConfig = cargarAchicada("recursos/fondo/fondo_configuracion_tierra.png",
                                         ANCHO_TIERRA_CONFIG, ALTO_TIERRA_CONFIG);

    texturaInicioApagado  = cargarAchicada("recursos/botones/boton_configuracion_inicio_A.png",
                                           ANCHO_BOTON_INICIO, ALTO_BOTON_INICIO);
    texturaInicioPrendido = cargarAchicada("recursos/botones/boton_configuracion_inicio_P.png",
                                           ANCHO_BOTON_INICIO, ALTO_BOTON_INICIO);

    // Al tamano de la partida; la vista previa de la configuracion lo dibuja
    // mas chico, y achicar no se nota.
    texturaHoyo = cargarAchicada("recursos/imagenes/hoyo.png", ANCHO_HOYO, ALTO_HOYO);

    texturaVida        = cargarAchicada("recursos/imagenes/vida.png",         LADO_VIDA, LADO_VIDA);
    texturaVidaPerdida = cargarAchicada("recursos/imagenes/vida_perdida.png", LADO_VIDA, LADO_VIDA);

    for(int i = 0; i < NUM_BOTONES_PAUSA_IMG; i++){
        texturaPausaApagado[i]  = cargarAchicada(
            TextFormat("recursos/botones/boton_pausa_%s_A.png", NOMBRES_PAUSA[i]),
            ANCHO_BOTON_PAUSA, ALTO_BOTON_PAUSA);
        texturaPausaPrendido[i] = cargarAchicada(
            TextFormat("recursos/botones/boton_pausa_%s_P.png", NOMBRES_PAUSA[i]),
            ANCHO_BOTON_PAUSA, ALTO_BOTON_PAUSA);
    }

    texturaFondoAjustes = cargarAchicada("recursos/fondo/fondo_ajustes.png",
                                         ANCHO_PANEL_AJUSTES, ALTO_PANEL_AJUSTES);
    texturaFondoFin     = cargarAchicada("recursos/fondo/fondo_FinalPartida.png",
                                         ANCHO_PANEL_FIN, ALTO_PANEL_FIN);
    texturaMapacheFin   = cargarAchicada("recursos/imagenes/mapache_baka.png",
                                         LADO_MAPACHE_FIN, LADO_MAPACHE_FIN);

    texturaVolumenMas   = cargarAchicada("recursos/botones/boton_volumen_mas.png",
                                         LADO_BOTON_VOLUMEN, LADO_BOTON_VOLUMEN);
    texturaVolumenMenos = cargarAchicada("recursos/botones/boton_volumen_menos.png",
                                         LADO_BOTON_VOLUMEN, LADO_BOTON_VOLUMEN);

    texturaCerrarApagado  = cargarAchicada("recursos/botones/boton_ajustes_cerrar_A.png",
                                           ANCHO_BOTON_CERRAR, ALTO_BOTON_CERRAR);
    texturaCerrarPrendido = cargarAchicada("recursos/botones/boton_ajustes_cerrar_P.png",
                                           ANCHO_BOTON_CERRAR, ALTO_BOTON_CERRAR);
}

void DescargarIconos()
{
    UnloadTexture(texturaRegresar);
    UnloadTexture(texturaPausa);
    UnloadTexture(texturaAjustes);

    UnloadTexture(texturaFondoPrincipal);
    UnloadTexture(texturaFondoJuego);
    UnloadTexture(texturaFondoConfiguracion);
    UnloadTexture(texturaFondoInstrucciones);
    UnloadTexture(texturaFondoCreditos);
    UnloadTexture(texturaFondoPuntaje);

    for(int i = 0; i < NUM_BOTONES_INTRO; i++){
        UnloadTexture(texturaIntroApagado[i]);
        UnloadTexture(texturaIntroPrendido[i]);
    }

    for(int i = 0; i < NUM_BOTONES_NIVEL; i++){
        UnloadTexture(texturaNivelApagado[i]);
        UnloadTexture(texturaNivelPrendido[i]);
    }

    UnloadTexture(texturaMarcoNombre);
    UnloadTexture(texturaTierraConfig);
    UnloadTexture(texturaInicioApagado);
    UnloadTexture(texturaInicioPrendido);
    UnloadTexture(texturaHoyo);

    UnloadTexture(texturaVida);
    UnloadTexture(texturaVidaPerdida);

    for(int i = 0; i < NUM_BOTONES_PAUSA_IMG; i++){
        UnloadTexture(texturaPausaApagado[i]);
        UnloadTexture(texturaPausaPrendido[i]);
    }

    UnloadTexture(texturaFondoAjustes);
    UnloadTexture(texturaFondoFin);
    UnloadTexture(texturaMapacheFin);
    UnloadTexture(texturaVolumenMas);
    UnloadTexture(texturaVolumenMenos);
    UnloadTexture(texturaCerrarApagado);
    UnloadTexture(texturaCerrarPrendido);
}

Texture2D iconoRegresar()
{
    return texturaRegresar;
}

Texture2D iconoPausa()
{
    return texturaPausa;
}

Texture2D iconoAjustes()
{
    return texturaAjustes;
}

Texture2D fondoPrincipal()
{
    return texturaFondoPrincipal;
}

Texture2D fondoJuego()
{
    return texturaFondoJuego;
}

Texture2D fondoConfiguracion()
{
    return texturaFondoConfiguracion;
}

Texture2D fondoInstrucciones()
{
    return texturaFondoInstrucciones;
}

Texture2D fondoCreditos()
{
    return texturaFondoCreditos;
}

Texture2D fondoPuntaje()
{
    return texturaFondoPuntaje;
}

Texture2D botonIntro(int indice, bool prendido)
{
    if(indice < 0 || indice >= NUM_BOTONES_INTRO) return Texture2D{};
    return prendido ? texturaIntroPrendido[indice] : texturaIntroApagado[indice];
}

Texture2D marcoNombre()
{
    return texturaMarcoNombre;
}

Texture2D fondoTierraConfig()
{
    return texturaTierraConfig;
}

Texture2D botonInicio(bool prendido)
{
    return prendido ? texturaInicioPrendido : texturaInicioApagado;
}

Texture2D imagenHoyo()
{
    return texturaHoyo;
}

Texture2D botonNivel(int indice, bool prendido)
{
    if(indice < 0 || indice >= NUM_BOTONES_NIVEL) return Texture2D{};
    return prendido ? texturaNivelPrendido[indice] : texturaNivelApagado[indice];
}

Texture2D iconoVida(bool perdida)
{
    return perdida ? texturaVidaPerdida : texturaVida;
}

Texture2D botonPausaImg(BotonPausa cual, bool prendido)
{
    if(cual < 0 || cual >= NUM_BOTONES_PAUSA_IMG) return Texture2D{};
    return prendido ? texturaPausaPrendido[cual] : texturaPausaApagado[cual];
}

Texture2D fondoAjustes()
{
    return texturaFondoAjustes;
}

Texture2D fondoFin()
{
    return texturaFondoFin;
}

Texture2D mapacheFin()
{
    return texturaMapacheFin;
}

Texture2D botonVolumenMas()
{
    return texturaVolumenMas;
}

Texture2D botonVolumenMenos()
{
    return texturaVolumenMenos;
}

Texture2D botonCerrarImg(bool prendido)
{
    return prendido ? texturaCerrarPrendido : texturaCerrarApagado;
}
