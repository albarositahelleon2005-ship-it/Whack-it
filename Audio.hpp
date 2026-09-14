/**
 * \file Audio.hpp
 * \brief La música de fondo y los efectos de sonido.
 * \date 13/09/2026
 *
 * raylib trae su propio módulo de audio y ya sabe leer mp3, así que este
 * módulo es una envoltura delgada: no reinventa nada, solo le pone nombre a
 * cada paso para que el resto del juego no tenga que saber que existe raylib.
 *
 * Hay dos clases de sonido y se manejan distinto a propósito:
 *   - La **música** (Music) se va leyendo del disco en pedazos mientras suena,
 *     porque es larga. Por eso necesita ActualizarAudio() en cada fotograma.
 *   - Los **efectos** (Sound) se cargan completos en memoria, porque son
 *     cortos y tienen que sonar al instante en que se piden.
 *
 * Archivos, todos en recursos/audio/:
 *   - VGM.mp3        música de fondo
 *   - sfx_golpe.mp3  suena al golpear a un enemigo
 *   - sfx_bomba.mp3  suena al golpear una bomba
 *
 * Para cambiar cualquiera de los tres basta con reemplazar el archivo.
 */

#ifndef AUDIO_HPP_INCLUDED
#define AUDIO_HPP_INCLUDED

/// Cuánto sube o baja el volumen con cada clic en la ventana de ajustes.
const float PASO_VOLUMEN = 0.1f;

/**
 * \brief Prende el dispositivo de audio y carga música y efectos. Llamar una
 * sola vez, después de InitWindow.
 */
void IniciarAudio();

/**
 * \brief Le da tiempo a la música para seguir sonando.
 *
 * Sin llamarla en cada fotograma, la música se corta a los pocos segundos.
 */
void ActualizarAudio();

/**
 * \brief Libera todo y apaga el dispositivo de audio. Llamar una sola vez,
 * antes de CloseWindow.
 */
void TerminarAudio();

/// El volumen actual de la música, de 0.0 a 1.0.
float volumenMusica();

/// El volumen actual de los efectos, de 0.0 a 1.0.
float volumenEfectos();

/**
 * \brief Sube o baja el volumen de la música.
 * \param delta Cuánto cambiarlo; negativo para bajarlo. Se recorta a [0, 1].
 */
void ajustarVolumenMusica(float delta);

/**
 * \brief Sube o baja el volumen de los efectos.
 * \param delta Cuánto cambiarlo; negativo para bajarlo. Se recorta a [0, 1].
 */
void ajustarVolumenEfectos(float delta);

/// Efecto de haberle pegado a un enemigo.
void sonarGolpe();

/// Efecto de haberle pegado a una bomba.
void sonarBomba();

#endif // AUDIO_HPP_INCLUDED
