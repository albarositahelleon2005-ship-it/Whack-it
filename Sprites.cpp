/**
 * \file Sprites.cpp
 * \brief Implementación de la carga de los GIFs del juego.
 * \date 13/09/2026
 */

#include "Sprites.hpp"

// Los cuadros por segundo salen del gif original: bomba.gif tiene sus cuadros
// a 60 ms (unos 16.7 por segundo) y enemigo_premium.gif a 200 ms (5). raylib
// no devuelve esos tiempos al cargar, asi que se anotan aqui. Si se cambia un
// gif por otro mas rapido o mas lento, este es el numero a tocar.
static const float FPS_BOMBA   = 16.7f;
static const float FPS_PREMIUM = 5.0f;

// El topo normal y las imagenes del golpe son fijos (un solo cuadro). Se
// cargan con el mismo cargarAnimacion que los gifs porque asi se achican
// igual, respetando la proporcion, y se dibujan con la misma funcion. Van en
// PNG y no en JPG porque la raylib incluida en libs/ no trae lector de JPG.
static const float FPS_FIJA = 1.0f;

static Animacion animEnemigo;
static Animacion animBomba;
static Animacion animPremium;
static Animacion animEnemigoAplastado;
static Animacion animPremiumAplastado;
static Animacion animBombaExplotada;

void CargarSprites()
{
    // El topo normal es un PNG de un solo cuadro, no un gif. La extension es
    // parte de la ruta: cambiar la imagen en recursos/ no basta si el archivo
    // nuevo trae otra extension, hay que corregirla aqui tambien.
    animEnemigo = cargarAnimacion("recursos/imagenes/enemigo.png",         LADO_SPRITE, FPS_FIJA);
    animBomba   = cargarAnimacion("recursos/imagenes/bomba.png",           LADO_SPRITE, FPS_BOMBA);
    animPremium = cargarAnimacion("recursos/imagenes/enemigo_premium.png", LADO_SPRITE, FPS_PREMIUM);

    animEnemigoAplastado = cargarAnimacion("recursos/imagenes/enemigo_derrotado.png",         LADO_SPRITE, FPS_FIJA);
    animPremiumAplastado = cargarAnimacion("recursos/imagenes/enemigo_premium_derrotado.png", LADO_SPRITE, FPS_FIJA);
    animBombaExplotada   = cargarAnimacion("recursos/imagenes/bomba_explosion.png",           LADO_SPRITE, FPS_FIJA);
}

void DescargarSprites()
{
    descargarAnimacion(animEnemigo);
    descargarAnimacion(animBomba);
    descargarAnimacion(animPremium);
    descargarAnimacion(animEnemigoAplastado);
    descargarAnimacion(animPremiumAplastado);
    descargarAnimacion(animBombaExplotada);
}

const Animacion& spriteEnemigo()
{
    return animEnemigo;
}

const Animacion& spriteBomba()
{
    return animBomba;
}

const Animacion& spritePremium()
{
    return animPremium;
}

const Animacion& spriteEnemigoAplastado()
{
    return animEnemigoAplastado;
}

const Animacion& spritePremiumAplastado()
{
    return animPremiumAplastado;
}

const Animacion& spriteBombaExplotada()
{
    return animBombaExplotada;
}
