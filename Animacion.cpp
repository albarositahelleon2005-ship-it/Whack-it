/**
 * \file Animacion.cpp
 * \brief Implementación de la carga de GIFs animados.
 * \date 13/09/2026
 */

#include <cmath>
#include <cstring>

#include "Animacion.hpp"

/**
 * \brief Copia un cuadro ya achicado dentro de la hoja, en su casilla.
 *
 * Se copia renglon por renglon con memcpy en vez de usar ImageDraw: las dos
 * imagenes estan en el mismo formato (R8G8B8A8), asi que aqui no hay nada que
 * mezclar ni convertir, y una copia cruda no puede ensuciar los bordes
 * transparentes del gif.
 *
 * \param hoja    Imagen grande donde se pega.
 * \param cuadro  Imagen chica ya del tamano correcto.
 * \param destinoX Esquina izquierda de la casilla dentro de la hoja.
 * \param destinoY Esquina superior de la casilla dentro de la hoja.
 */
static void pegarCuadro(Image& hoja, const Image& cuadro, int destinoX, int destinoY)
{
    unsigned char* pixelesHoja   = (unsigned char*)hoja.data;
    unsigned char* pixelesCuadro = (unsigned char*)cuadro.data;

    const int BYTES = 4;   // R, G, B y A, un byte cada uno

    for(int y = 0; y < cuadro.height; y++){

        unsigned char* origen  = pixelesCuadro + (size_t)y * cuadro.width * BYTES;
        unsigned char* destino = pixelesHoja
                               + ((size_t)(destinoY + y) * hoja.width + destinoX) * BYTES;

        memcpy(destino, origen, (size_t)cuadro.width * BYTES);
    }
}

Animacion cargarAnimacion(const char* ruta, int lado, float cuadrosPorSegundo)
{
    Animacion animacion;
    animacion.cuadros        = 0;
    animacion.columnas       = 0;
    animacion.lado           = lado;
    animacion.duracionCuadro = (cuadrosPorSegundo > 0.0f) ? (1.0f / cuadrosPorSegundo) : 0.1f;
    animacion.hoja.id        = 0;

    int cuadros = 0;
    Image tira = LoadImageAnim(ruta, &cuadros);

    // Si el archivo no esta o no se pudo leer, se regresa la animacion vacia.
    // El juego lo revisa con animacionLista() y dibuja un relleno: perder el
    // gif no deberia tumbar el programa entero.
    if(tira.data == NULL || cuadros <= 0){
        if(tira.data != NULL) UnloadImage(tira);
        return animacion;
    }

    // LoadImageAnim deja todos los cuadros pegados uno tras otro dentro de
    // tira.data, pero tira.width/height son los de UN cuadro. Ese es el dato
    // clave para poder recorrerlos.
    const int ANCHO = tira.width;
    const int ALTO  = tira.height;
    const int BYTES_POR_CUADRO = ANCHO * ALTO * 4;

    // La hoja se arma como cuadricula y no como una tira larga: 101 cuadros de
    // 128 px en una sola fila darian 12928 px de ancho, y muchas tarjetas de
    // video no aceptan texturas tan grandes. Una cuadricula de 11x10 si.
    int columnas = (int)ceil(sqrt((double)cuadros));
    int filas    = (cuadros + columnas - 1) / columnas;

    Image hoja = GenImageColor(columnas * lado, filas * lado, BLANK);

    // Cuanto hay que achicar para que el cuadro quepa en el lado pedido sin
    // deformarse: manda el eje mas grande, y lo que sobra del otro queda
    // transparente a los lados.
    //
    // Se dejan 2 px de margen transparente alrededor de cada casilla. Suena a
    // detalle, pero es lo que evita el clasico defecto de las hojas de sprites:
    // al dibujar con suavizado, la tarjeta de video promedia con los pixeles
    // vecinos, y sin ese margen el cuadro de al lado se asoma por la orilla.
    const int MARGEN_CASILLA = 2;

    int util = lado - MARGEN_CASILLA * 2;
    if(util < 1) util = 1;

    float escala = (float)util / (float)((ANCHO > ALTO) ? ANCHO : ALTO);

    int nuevoAncho = (int)(ANCHO * escala);
    int nuevoAlto  = (int)(ALTO  * escala);
    if(nuevoAncho < 1) nuevoAncho = 1;
    if(nuevoAlto  < 1) nuevoAlto  = 1;

    int margenX = (lado - nuevoAncho) / 2;
    int margenY = (lado - nuevoAlto ) / 2;

    for(int i = 0; i < cuadros; i++){

        // Un Image "prestado" que apunta al cuadro i dentro de tira.data. No
        // es dueno de esa memoria, por eso NUNCA se le llama UnloadImage: la
        // memoria se libera de un solo golpe al final, con la tira completa.
        Image prestado;
        prestado.data    = (unsigned char*)tira.data + (size_t)i * BYTES_POR_CUADRO;
        prestado.width   = ANCHO;
        prestado.height  = ALTO;
        prestado.mipmaps = 1;
        prestado.format  = tira.format;

        // ImageCopy si reserva memoria propia, y esa copia es la que se puede
        // achicar sin tocar el original.
        Image copia = ImageCopy(prestado);

        // El gif deberia venir ya en R8G8B8A8, pero asegurarlo aqui es lo que
        // permite que pegarCuadro use memcpy sin preguntar nada.
        ImageFormat(&copia, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        ImageResize(&copia, nuevoAncho, nuevoAlto);

        int columna = i % columnas;
        int fila    = i / columnas;

        pegarCuadro(hoja, copia, columna * lado + margenX, fila * lado + margenY);

        UnloadImage(copia);
    }

    animacion.hoja     = LoadTextureFromImage(hoja);
    animacion.cuadros  = cuadros;
    animacion.columnas = columnas;

    // Suaviza el gif al estirarlo o encogerlo, para que no se vea a bloques.
    SetTextureFilter(animacion.hoja, TEXTURE_FILTER_BILINEAR);

    // Las dos imagenes de RAM ya no hacen falta: lo que se dibuja vive en la
    // tarjeta de video. Aqui es donde se sueltan esos 130 MB de bomba.gif.
    UnloadImage(hoja);
    UnloadImage(tira);

    return animacion;
}

void descargarAnimacion(Animacion& animacion)
{
    if(animacion.cuadros > 0){
        UnloadTexture(animacion.hoja);
    }

    animacion.cuadros = 0;
}

bool animacionLista(const Animacion& animacion)
{
    return (animacion.cuadros > 0);
}

void dibujarAnimacion(const Animacion& animacion, Rectangle destino, float tiempo)
{
    if(!animacionLista(animacion)) return;

    // Que cuadro toca: el tiempo dividido entre lo que dura cada uno, dando la
    // vuelta al llegar al final. El modulo es lo que hace que el gif se repita
    // solo, sin guardar ningun contador.
    int indice = (int)(tiempo / animacion.duracionCuadro);
    if(indice < 0) indice = 0;
    indice = indice % animacion.cuadros;

    int columna = indice % animacion.columnas;
    int fila    = indice / animacion.columnas;

    Rectangle origen = { (float)(columna * animacion.lado),
                         (float)(fila    * animacion.lado),
                         (float)animacion.lado,
                         (float)animacion.lado };

    Vector2 sinDesfase = { 0.0f, 0.0f };

    DrawTexturePro(animacion.hoja, origen, destino, sinDesfase, 0.0f, WHITE);
}
