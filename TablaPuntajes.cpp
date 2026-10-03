/**
 * \file TablaPuntajes.cpp
 * \brief Implementación de la tabla de mejores puntajes.
 * \date 03/10/2026
 */

#include <cstdio>
#include <cstring>

#include "TablaPuntajes.hpp"

void vaciarTabla(TablaPuntajes& tabla)
{
    for(int nivel = 0; nivel < NUM_DIFICULTADES; nivel++){
        tabla.cantidad[nivel] = 0;
    }
}

/**
 * \brief Si el registro nuevo le gana al que ya ocupa un lugar.
 *
 * Empatar no basta: asi, a igualdad de todo, el que ya estaba conserva su
 * lugar y nadie lo "baja" con el mismo resultado.
 */
static bool leGana(int puntos, int racha, const RegistroPuntaje& otro)
{
    if(puntos != otro.puntos) return puntos > otro.puntos;
    return racha > otro.mejorRacha;
}

int anotarEnTabla(TablaPuntajes& tabla, Dificultad nivel,
                  const char* nombre, int puntos, int mejorRacha)
{
    if(nivel < 0 || nivel >= NUM_DIFICULTADES) return -1;

    RegistroPuntaje* lugares  = tabla.lugares[nivel];
    int&             cantidad = tabla.cantidad[nivel];

    // Con 10 lugares, buscar el sitio de uno en uno y recorrer los de abajo
    // es mas claro que cualquier ordenamiento, y son a lo mas 10 pasos.
    int lugar = 0;
    while(lugar < cantidad && !leGana(puntos, mejorRacha, lugares[lugar])){
        lugar++;
    }

    if(lugar >= LUGARES_POR_NIVEL) return -1;

    // Se recorren hacia abajo los que quedan debajo; si la tabla ya estaba
    // llena, el ultimo se cae.
    int ultimo = (cantidad < LUGARES_POR_NIVEL) ? cantidad : LUGARES_POR_NIVEL - 1;

    for(int i = ultimo; i > lugar; i--){
        lugares[i] = lugares[i - 1];
    }

    strncpy(lugares[lugar].nombre, nombre, NOMBRE_MAX);
    lugares[lugar].nombre[NOMBRE_MAX] = '\0';
    lugares[lugar].puntos     = puntos;
    lugares[lugar].mejorRacha = mejorRacha;

    if(cantidad < LUGARES_POR_NIVEL) cantidad++;

    return lugar;
}

void cargarTabla(TablaPuntajes& tabla, const char* ruta)
{
    vaciarTabla(tabla);

    FILE* archivo = fopen(ruta, "r");
    if(archivo == nullptr) return;

    char renglon[128];

    while(fgets(renglon, sizeof(renglon), archivo) != nullptr){

        int  nivel = 0, puntos = 0, racha = 0;
        char nombre[NOMBRE_MAX + 1] = "";

        // %12[^\n] = "hasta 12 caracteres, lo que sea menos el salto de
        // renglon": asi el nombre puede llevar espacios. El 12 tiene que
        // coincidir con NOMBRE_MAX.
        if(sscanf(renglon, "%d %d %d %12[^\n]", &nivel, &puntos, &racha, nombre) != 4) continue;
        if(nivel < 0 || nivel >= NUM_DIFICULTADES) continue;

        // Se meten con anotarEnTabla y no copiandolos tal cual: si alguien
        // edito el archivo a mano y lo desordeno, la tabla igual queda bien.
        anotarEnTabla(tabla, (Dificultad)nivel, nombre, puntos, racha);
    }

    fclose(archivo);
}

bool guardarTabla(const TablaPuntajes& tabla, const char* ruta)
{
    FILE* archivo = fopen(ruta, "w");
    if(archivo == nullptr) return false;

    for(int nivel = 0; nivel < NUM_DIFICULTADES; nivel++){
        for(int i = 0; i < tabla.cantidad[nivel]; i++){
            const RegistroPuntaje& r = tabla.lugares[nivel][i];
            fprintf(archivo, "%d %d %d %s\n", nivel, r.puntos, r.mejorRacha, r.nombre);
        }
    }

    fclose(archivo);
    return true;
}
