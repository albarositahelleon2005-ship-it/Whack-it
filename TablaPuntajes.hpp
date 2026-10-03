/**
 * \file TablaPuntajes.hpp
 * \brief Los 10 mejores puntajes de cada nivel, y cómo se guardan en disco.
 * \date 03/10/2026
 *
 * Igual que Partida.hpp, este módulo **no toca raylib**: solo arreglos y
 * archivos de C. Así se puede probar con un main() de consola (ver CLAUDE.md,
 * *Pruebas*), y la pantalla de puntajes (Puntaje.cpp) solo se encarga de
 * dibujar lo que hay aquí.
 *
 * El archivo es de texto, un renglón por puntaje:
 *
 *     nivel puntos racha nombre
 *
 * con el nivel como número (0 fácil, 1 normal, 2 difícil). El nombre va al
 * final porque puede llevar espacios: es "lo que quede del renglón". Que sea
 * texto y no binario es a propósito: si algo sale mal en la feria, se puede
 * abrir con el Bloc de notas y corregir o borrar a mano.
 */

#ifndef TABLAPUNTAJES_HPP_INCLUDED
#define TABLAPUNTAJES_HPP_INCLUDED

#include "ConfigPartida.hpp"
#include "Dificultad.hpp"

/// Cuántos lugares guarda cada nivel.
const int LUGARES_POR_NIVEL = 10;

/**
 * \brief Un renglón de la tabla: quién jugó y cómo le fue.
 */
struct RegistroPuntaje {
    char nombre[NOMBRE_MAX + 1];   ///< Nombre del jugador
    int  puntos;                   ///< Puntaje final (los mapaches atrapados)
    int  mejorRacha;               ///< La racha más larga de esa partida
};

/**
 * \brief Los mejores puntajes de los tres niveles, ya ordenados.
 *
 * En cada nivel el lugar 0 es el primero. Solo los primeros
 * cantidad[nivel] lugares tienen datos.
 */
struct TablaPuntajes {
    RegistroPuntaje lugares[NUM_DIFICULTADES][LUGARES_POR_NIVEL];
    int             cantidad[NUM_DIFICULTADES];
};

/**
 * \brief Deja la tabla vacía, sin ningún puntaje.
 */
void vaciarTabla(TablaPuntajes& tabla);

/**
 * \brief Mete un puntaje en su lugar, si le alcanza para entrar al top.
 *
 * Gana el de más puntos; si empatan, el de racha más larga; si también
 * empatan, se queda arriba el que llegó primero (el nuevo va debajo).
 *
 * \return El lugar en que quedó (0 es el primero), o -1 si no entró.
 */
int anotarEnTabla(TablaPuntajes& tabla, Dificultad nivel,
                  const char* nombre, int puntos, int mejorRacha);

/**
 * \brief Lee la tabla de un archivo.
 *
 * Si el archivo no existe (la primera vez que se juega) la tabla queda
 * vacía. Los renglones que no se entiendan se saltan, sin tronar.
 */
void cargarTabla(TablaPuntajes& tabla, const char* ruta);

/**
 * \brief Escribe la tabla completa en un archivo, reemplazando lo que hubiera.
 * \return Falso si no se pudo escribir (por ejemplo, carpeta de solo lectura).
 */
bool guardarTabla(const TablaPuntajes& tabla, const char* ruta);

#endif // TABLAPUNTAJES_HPP_INCLUDED
