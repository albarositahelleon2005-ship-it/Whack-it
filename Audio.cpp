/**
 * \file Audio.cpp
 * \brief Implementación de la música y los efectos de sonido.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Audio.hpp"

static Music vgm;
static Sound sfxGolpe;
static Sound sfxBomba;

// Los volumenes viven aqui, en floats propios, y no se le preguntan a raylib:
// raylib deja ponerlos pero no deja consultarlos, y la ventana de ajustes
// necesita saber en cuanto van para dibujar la barra.
static float nivelMusica  = 0.6f;
static float nivelEfectos = 0.8f;

/**
 * \brief Recorta un valor al rango [0, 1].
 *
 * Sube y baja de volumen se piden con sumas y restas, asi que sin esto el
 * nivel se iria de rango a la quinta vez que alguien pique el boton.
 */
static float recortar(float valor)
{
    if(valor < 0.0f) return 0.0f;
    if(valor > 1.0f) return 1.0f;
    return valor;
}

/**
 * \brief Le pasa a raylib los volumenes que tenemos guardados.
 */
static void aplicarVolumenes()
{
    SetMusicVolume(vgm, nivelMusica);
    SetSoundVolume(sfxGolpe, nivelEfectos);
    SetSoundVolume(sfxBomba, nivelEfectos);
}

void IniciarAudio()
{
    InitAudioDevice();

    vgm = LoadMusicStream("recursos/audio/VGM.mp3");
    vgm.looping = true;

    sfxGolpe = LoadSound("recursos/audio/sfx_golpe.mp3");
    sfxBomba = LoadSound("recursos/audio/sfx_bomba.mp3");

    aplicarVolumenes();

    PlayMusicStream(vgm);
}

void ActualizarAudio()
{
    UpdateMusicStream(vgm);
}

void TerminarAudio()
{
    UnloadSound(sfxBomba);
    UnloadSound(sfxGolpe);
    UnloadMusicStream(vgm);

    CloseAudioDevice();
}

float volumenMusica()
{
    return nivelMusica;
}

float volumenEfectos()
{
    return nivelEfectos;
}

void ajustarVolumenMusica(float delta)
{
    nivelMusica = recortar(nivelMusica + delta);
    SetMusicVolume(vgm, nivelMusica);
}

void ajustarVolumenEfectos(float delta)
{
    nivelEfectos = recortar(nivelEfectos + delta);

    SetSoundVolume(sfxGolpe, nivelEfectos);
    SetSoundVolume(sfxBomba, nivelEfectos);
}

void sonarGolpe()
{
    // Si el archivo no estuviera, IsSoundValid dice que no y no se intenta
    // reproducir: falta un sonido, no se cae el juego.
    if(IsSoundValid(sfxGolpe)) PlaySound(sfxGolpe);
}

void sonarBomba()
{
    if(IsSoundValid(sfxBomba)) PlaySound(sfxBomba);
}
