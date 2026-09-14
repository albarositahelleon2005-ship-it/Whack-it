# Whack it — contexto del proyecto

Juego tipo *golpea-al-topo* en C++ con **raylib 6.0**, proyecto escolar de la
Licenciatura en Ciencias de la Computación (Universidad de Sonora).

- **Entorno de compilación:** Code::Blocks + MinGW **64 bits en Windows**.
  El proyecto es `WhackIt.cbp`. La raylib viene incluida en `libs/raylib/`
  (`libraylib.a` estático + headers), no se instala nada aparte.
- **Idioma:** todo el código, los comentarios y la documentación Doxygen van en
  **español**. Mantener ese estilo.
- **Antes se llamaba** `JuegoRitmico` y era un esqueleto de juego rítmico. Se
  reconvirtió a *Whack it*. Si aparece esa palabra en algún lado, es residuo.

---

## Compilar y correr

```
Code::Blocks → abrir WhackIt.cbp
  objetivo "Debug"    → con consola, para ver los TRACELOG de raylib
  objetivo "Release"  → -mwindows -static, sin consola, para repartir
Build → Rebuild
```

El ejecutable queda en `bin\Debug\` o `bin\Release\`.
**El `.exe` necesita la carpeta `recursos/` a un lado**; `main()` hace
`ChangeDirectory(GetApplicationDirectory())` para que eso funcione aunque se
abra desde un acceso directo.

`empaquetar.bat` arma `Distribuir\WhackIt.zip` a partir del build Release.
`instalador.iss` es un guion opcional de Inno Setup.

---

## Arquitectura

Es el patrón clásico de raylib: **modo inmediato**, sin objetos que se
registren ni se liberen. Cada pantalla expone exactamente dos funciones:

```cpp
Escena_Estado ActualizarX();   // lee entrada y decide; NO dibuja
void          DibujarX();      // dibuja el estado; NO lo modifica
```

`main.cpp` guarda un `Escena_Estado` y con un `switch` decide qué actualizar y
qué dibujar en cada vuelta del bucle. Reglas que hay que respetar:

1. **Actualizar y dibujar están separados.** Leer la posición del mouse al
   dibujar está bien; *decidir* algo al dibujar, no.
2. **Tres cosas NO son escenas**, son ventanas virtuales que se dibujan encima
   de lo que haya debajo (que se sigue viendo, oscurecido con `COLOR_VELO`):
   - **Pausa** y **FIN de partida** → encima del juego, las maneja `Juego.cpp`.
   - **Ajustes de volumen** → encima de cualquier pantalla, la maneja `main.cpp`.
   Por eso no aparecen en el `enum Escena_Estado`.
3. **La geometría vive en un solo lugar.** `VistaTablero.hpp` calcula dónde va
   cada pozo; el dibujo y la detección del clic preguntan ahí. Nunca duplicar
   coordenadas.
4. **`Partida.cpp` no toca raylib.** Recibe el `dt` por parámetro y devuelve un
   `ResultadoGolpe`; quien reproduce el sonido es `Juego.cpp`. Esto es a
   propósito y **conviene no romperlo**: permite probar toda la lógica del juego
   con un `main()` de consola en Linux/Windows sin ventana (ver *Pruebas*).
5. **Solo ASCII en los strings que se dibujan.** La fuente de fábrica de raylib
   no cubre acentos ni eñes: saldrían cuadritos. Los comentarios sí llevan
   acentos, los `DrawText` no.
6. Los comentarios explican **por qué**, no qué. Ese es el estilo del proyecto.

### Mapa de archivos

| Archivo | Qué es |
|---|---|
| `main.cpp` | Bucle principal, ruteo de escenas, barra superior, overlay de ajustes |
| `Escena.hpp` | El `enum` de pantallas |
| `Menu.cpp` | Menú principal |
| `Configuracion.cpp` | Nombre del jugador (máx 12) + dificultad + vista previa de pozos |
| `Juego.cpp` | Pantalla de partida: traduce entrada y dibujo, nada más |
| **`Partida.cpp`** | **Las reglas del juego.** Puntaje, vidas, racha, reloj de apariciones |
| `Tablero.cpp` | Modelo de los pozos y qué asoma en cada uno |
| `VistaTablero.cpp` | Geometría de los pozos, dibujo del tablero y del marcador |
| `Dificultad.cpp` | Tabla de `ReglasDificultad`. **Aquí se ajusta todo el balance** |
| `Pausa.cpp` / `Resultados.cpp` / `Ajustes.cpp` | Las tres ventanas virtuales |
| `Animacion.cpp` | Convierte un GIF animado en hoja de sprites (ver abajo) |
| `Sprites.cpp` | Carga `enemigo.gif` y `bomba.gif` una sola vez |
| `Iconos.cpp` | Carga los PNG de botones |
| `Audio.cpp` | Música + 2 efectos, con volúmenes independientes |
| `Boton.cpp` / `Dibujo.cpp` / `Aleatorio.cpp` | Utilidades compartidas |
| `icono.rc` | Mete el ícono y los datos de versión dentro del `.exe` |

---

## Reglas del juego (tal como están implementadas)

- Se golpea con **clic izquierdo** del ratón.
- **Topo golpeado:** +1 punto, la racha crece.
- **Bomba golpeada:** −2 puntos, −1 vida, la racha se pierde.
- **Topo que se esconde solo:** se pierde la racha, **no** se pierde vida.
- El **puntaje nunca baja de cero**.
- Golpear un pozo vacío o el fondo **no castiga**.
- La partida **termina al quedarse sin vidas**. Sale la ventana de FIN con
  puntaje y combo más largo.
- El contador de combo **desaparece de la pantalla** cuando la racha es 0.

### Tabla de dificultad (`reglasDe()` en `Dificultad.cpp`)

| Nivel | Pozos | Vidas | Visible | Baja hasta | Apariciones | Bombas |
|---|---|---|---|---|---|---|
| Fácil | 5 | 1 | 3.0 s | 1.5 s | uno a la vez | 20 % |
| Normal | 7 | 2 | 2.5 s | 1.5 s | uno a la vez | 25 % |
| Difícil | 10 | 3 | 2.0 s | 1.0 s | uno cada 1 s, pueden coincidir | 30 % |

El tiempo visible baja `pasoReduccion` en cada aparición hasta tocar el piso.
`APARICIONES_PARA_ACELERAR` (= 25) es el único número que controla qué tan
rápido se acelera la curva.

> Nota: que el modo fácil tenga **menos** vidas que el difícil es intencional,
> viene del boceto original del equipo (1, 2 y 3 corazones). No "arreglarlo"
> sin preguntar.

---

## Los GIFs

`Animacion.cpp` resuelve un problema concreto: `LoadImageAnim()` de raylib deja
todos los cuadros en RAM a tamaño original — `bomba.gif` son 638×550 × 99
cuadros ≈ **130 MB**. Así que al arrancar se achica cada cuadro a 128 px
(respetando proporción, centrado, con 2 px de margen transparente para que el
filtrado bilineal no chupe píxeles de la casilla vecina), se acomodan en una
hoja tipo cuadrícula y se sube una sola textura a la GPU. Después, animar es
puro recorte por tiempo.

- La hoja es cuadrícula y no tira horizontal porque 101 cuadros × 128 px darían
  12928 px de ancho y muchas GPU no aceptan texturas así.
- Los FPS de cada gif están escritos a mano en `Sprites.cpp` (raylib no
  devuelve los tiempos del gif): enemigo 10 fps, bomba 16.7 fps.
- Si un `.gif` falta, `animacionLista()` da falso y se dibuja un círculo de
  color en su lugar. El juego no se cae.

**Pendiente conocido:** `bomba.gif` no tiene transparencia (es RGB), así que se
ve como un rectángulo con el fondo de la escena original saliendo del pozo.
`enemigo.gif` sí tiene alfa y se ve perfecto. Solución real: conseguir un gif de
bomba con fondo transparente.

---

## Pruebas

`Partida.cpp`, `Tablero.cpp`, `Dificultad.cpp` y `Aleatorio.cpp` **no incluyen
raylib**, así que se pueden compilar y correr solos:

```
g++ -std=c++11 -Wall -Wextra -o prueba prueba.cpp \
    Partida.cpp Tablero.cpp Dificultad.cpp Aleatorio.cpp
```

Ya se validó así: pozos/vidas por nivel, recorte del nombre a 12 caracteres,
"uno a la vez" en fácil/normal, simultáneos en difícil, +1 por topo, la curva de
aceleración tocando el piso, escape sin perder vida, bomba (−2, −1 vida, racha
a 0), puntaje que no va a negativos, fin de partida y proporción de bombas.
**Si se tocan las reglas, conviene rehacer esa prueba.**

---

## Lo que sigue pendiente

- `Puntaje.cpp` (mejores puntajes), `Creditos.cpp` y `Opciones.cpp` siguen como
  pantallas de relleno con `dibujarPantallaPendiente()`. `Escena_opciones` ni
  siquiera tiene entrada desde el menú.
- No se guarda nada en disco: los puntajes se pierden al cerrar.
- `Monticulo.hpp` está reservado a propósito y **vacío**. Se decidió no meter un
  montículo porque con 10 pozos revisarlos todos cada fotograma son 10
  comparaciones; el archivo tiene la explicación. Si el profe lo pide, ahí va.
- `recursos/imagenes/icono_sonido.png` quedó sin usar (lo reemplazó el engrane
  de ajustes).
- Los dos `sfx_*.mp3` son sonidos generados de relleno, se pueden reemplazar por
  otros mejores sin tocar código.
