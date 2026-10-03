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
abra desde un acceso directo. Si ahí no hay `recursos/`, sube hasta dos
carpetas (así el `.exe` de `bin\Debug\` encuentra la del proyecto). El `.cbp`
además copia `recursos/` junto al `.exe` después de cada build (`cmd /c xcopy`:
Code::Blocks no corre esos pasos dentro de `cmd`, así que sin `cmd /c` la
redirección `>nul` rompía la copia).

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
5. **Solo ASCII en los strings que se dibujan.** Ni la fuente ARCO (94 glifos)
   ni la de fábrica de raylib cubren acentos ni eñes: saldrían cuadritos. Los
   comentarios sí llevan acentos, los `dibujarTexto` no.
6. Los comentarios explican **por qué**, no qué. Ese es el estilo del proyecto.

### Mapa de archivos

| Archivo | Qué es |
|---|---|
| `main.cpp` | Bucle principal, ruteo de escenas, barra superior, overlay de ajustes |
| `Escena.hpp` | El `enum` de pantallas |
| `Menu.cpp` | Menú principal |
| `Configuracion.cpp` | Nombre del jugador (máx 12; vuelve a "Player 1" cada vez que se entra desde el menú, lo hace `main.cpp`) + dificultad + vista previa de pozos. Fila resaltada con flechas o mouse, como el menú |
| `Juego.cpp` | Pantalla de partida: traduce entrada y dibujo, nada más |
| **`Partida.cpp`** | **Las reglas del juego.** Puntaje, vidas, racha, reloj de apariciones |
| `Tablero.cpp` | Modelo de los pozos y qué asoma en cada uno |
| `VistaTablero.cpp` | Geometría de los pozos, dibujo del tablero y del marcador |
| `Dificultad.cpp` | Tabla de `ReglasDificultad`. **Aquí se ajusta todo el balance** |
| `Pausa.cpp` / `Resultados.cpp` / `Ajustes.cpp` | Las tres ventanas virtuales |
| `Puntaje.cpp` | Pantalla de mejores puntajes: botones de nivel + top 10 (nombre, racha más larga, mapaches atrapados = puntaje, en naranja) |
| `TablaPuntajes.cpp` | El top 10 de cada nivel y el archivo `puntajes.txt` (texto, junto a `recursos/`). Sin raylib. Se anota y guarda al terminar cada partida (`Juego.cpp`) |
| `Animacion.cpp` | Convierte un GIF animado en hoja de sprites (ver abajo) |
| `Sprites.cpp` | Carga `enemigo.png`, `bomba.gif` y `enemigo_premium.gif` una sola vez |
| `Iconos.cpp` | Carga los PNG de botones |
| `Audio.cpp` | Música + 2 efectos, con volúmenes independientes |
| `Boton.cpp` / `Dibujo.cpp` / `Aleatorio.cpp` | Utilidades compartidas |
| `icono.rc` | Mete el ícono y los datos de versión dentro del `.exe` |

---

## Reglas del juego (tal como están implementadas)

- Se golpea con **clic izquierdo** del ratón. Durante la partida el teclado
  solo sirve para ESC (pausa). La pausa y los ajustes también se manejan con
  flechas + Enter (en ajustes, izquierda/derecha cambian el volumen); la
  ventana de FIN es solo con mouse.
- **Topo golpeado:** +1 punto, la racha crece.
- **Topo dorado golpeado:** +`PUNTOS_PREMIUM` (= 1, igual que el normal; estuvo
  en 3 y el equipo pidió regresarlo), la racha crece.
- **Bomba golpeada:** −2 puntos, −1 vida, la racha se pierde.
- Lo golpeado se queda **0.5 s** (`DURACION_APLASTADO`) en su pozo con la imagen
  de aplastado/explosión. Mientras, el pozo sigue ocupado: no sale nada encima,
  cuenta para el tope de 2 y volver a golpearlo no hace nada.
- **Topo (normal o dorado) que se esconde solo:** se pierde la racha, y **cada 3
  escapes en la partida** (no seguidos, `ESCAPES_POR_VIDA`) se pierde una vida.
  El marcador muestra "Escapes: x/3"; al tercero se queda 1 s en "3/3" en
  naranja (`DURACION_AVISO_ESCAPES`) mientras se rompe el corazón. Sin ese
  aviso pasaba de "2/3" a "0/3" de golpe y parecía que la vida se perdía
  hasta el cuarto escape.
- El **puntaje nunca baja de cero**.
- Golpear un pozo vacío o el fondo **no castiga**.
- La partida **termina al quedarse sin vidas**. Sale la ventana de FIN con
  puntaje y combo más largo.
- El contador de combo **desaparece de la pantalla** cuando la racha es 0.

### Topo dorado (`porcentajePremium()` en `Partida.cpp`)

Se cuenta cuántos objetos (topos y bombas) han salido desde el último dorado.
15 % normalmente; 50 % pasados 20 sin dorado; el objeto número 30 es dorado
seguro. Al salir un dorado la cuenta vuelve a 0. Los números están en
`Dificultad.hpp`.

### Tabla de dificultad (`reglasDe()` en `Dificultad.cpp`)

| Nivel | Pozos | Vidas | Visible | Baja hasta | Bombas |
|---|---|---|---|---|---|
| Fácil | 5 | 3 | 3.0 s | 1.5 s | 20 % |
| Normal | 7 | 2 | 2.0 s | 1.0 s | 25 % |
| Difícil | 10 | 1 | 1.5 s | 0.5 s | 30 % |

En los tres: máximo **2 objetos a la vez** (`maxSimultaneos`), y el tiempo
visible baja **0.2 s cada 10 apariciones** (`pasoReduccion`,
`aparicionesPorPaso`) hasta tocar el piso. Sale un objeto nuevo cada
`visibleActual / maxSimultaneos` segundos si hay lugar; si ya hay 2, sale en
cuanto se libere un pozo.

---

## Los GIFs

`Animacion.cpp` resuelve un problema concreto: `LoadImageAnim()` de raylib deja
todos los cuadros en RAM a tamaño original — `bomba.gif` son 638×550 × 99
cuadros ≈ **130 MB**. Así que al arrancar se achica cada cuadro a 160 px (`LADO_SPRITE`, un poco más que los 150 px a los que se dibuja)
(respetando proporción, centrado, con 2 px de margen transparente para que el
filtrado bilineal no chupe píxeles de la casilla vecina), se acomodan en una
hoja tipo cuadrícula y se sube una sola textura a la GPU. Después, animar es
puro recorte por tiempo.

- La hoja es cuadrícula y no tira horizontal porque 101 cuadros × 128 px darían
  12928 px de ancho y muchas GPU no aceptan texturas así.
- Los FPS de cada gif están escritos a mano en `Sprites.cpp` (raylib no
  devuelve los tiempos del gif): bomba 16.7 fps, dorado 5 fps.
- El topo normal (`enemigo.png`) y las imágenes de golpe (`enemigo_derrotado`,
  `enemigo_premium_derrotado`, `bomba_explosion`) son de un solo cuadro y pasan
  por el mismo `cargarAnimacion()`. Van en **PNG**: la `libraylib.a` incluida no
  trae lector de JPG (solo PNG, GIF, BMP, QOI, DDS), así que un `.jpg` no carga.
- Si una imagen falta, `animacionLista()` da falso y se dibuja un círculo de
  color en su lugar. El juego no se cae.

**Pendiente conocido:** `bomba.gif`, `enemigo_premium.gif` y las 3 imágenes de
golpe no tienen transparencia, así que se ven como un rectángulo con su fondo
saliendo del pozo. `enemigo.png` sí tiene alfa y se ve perfecto. Solución real:
conseguir esas imágenes con fondo transparente.

---

## Botones y fondos

- `recursos/botones/` trae los botones de madera; `recursos/fondo/` un fondo
  por pantalla (puntaje ya usa el suyo, aunque su contenido sigue de relleno) (`fondo_juego.png` es un boceto en blanco que no se usa; la
  partida usa `fondo_partida.png`).
- Los botones con versión `_A` (apagado) y `_P` (prendido): el seleccionado
  se dibuja con `_P` y los demás con `_A`.
- El arte llega exportado enorme (hasta 21667×12506: más de 1 GB de RAM al
  descomprimir). Las copias de `recursos/` están **achicadas a ~2× su tamaño en
  pantalla**; los originales quedan en `arte_original/`. Si llega arte nuevo,
  achicarlo igual antes de meterlo: si no, el arranque tarda segundos.
  Además `Iconos.cpp` los vuelve a ajustar al cargar, al tamaño exacto en que
  se dibujan (`ANCHO_*`/`ALTO_*` en `Iconos.hpp`).
- `dibujarBotonImagen()` (`Boton.cpp`) dibuja un botón de imagen y, si falta,
  uno de texto en su lugar.
- La pausa **no tiene panel**: solo el velo, el ícono grande de pausa (adorno,
  no se pica) y los 3 botones. Ajustes y FIN usan `fondo_ajustes.png` y
  `fondo_FinalPartida.png`, que ya traen su título dibujado. En FIN va
  `mapache_baka.png` a la izquierda y los resultados centrados a su derecha.
- Créditos: el texto está en `Creditos.cpp`, sin acentos ni eñes ("Ninez")
  por la fuente. El nombre del evento y los encabezados (Desarrolladores,
  Artistas) van en `COLOR_NARANJA` (#FB6334, el de los letreros).
- Si falta la `_A` de un nivel, se dibuja la `_P` oscurecida.
- `fondo_configuracion_tierra.png` es el parche de tierra de la vista previa
  de pozos. Llegó como lienzo de 1280×720 con fondo blanco opaco: la copia de
  `recursos/` está recortada y con ese blanco hecho transparente. Se dibuja
  centrado a `ANCHO_TIERRA_CONFIG`×`ALTO_TIERRA_CONFIG` (`Iconos.hpp`): 200 de alto,
  el ancho proporcional. El resumen de pozos/vidas va encima, no adentro.
- El botón Inicio de la configuración es `boton_configuracion_inicio_A/_P`;
  sin nombre válido se dibuja la `_A` oscurecida.
- Los íconos de la barra (regresar, pausa, ajustes) **crecen** `CRECE_ICONO`
  px con el mouse encima, sin fondo. Se cargan a 128 px para que crecer no
  se vea borroso.
- `hoyo.png` es solo el montículo de enfrente: cada pozo dibuja su muñeco y
  **después** su hoyo encima, en orden de índice (fila de arriba primero).
  Mide `ANCHO_HOYO`×`ALTO_HOYO`, que es también el tamaño del pozo.

### Tipografía y colores

- Todo el texto pasa por `dibujarTexto()` / `medirTexto()` (`Dibujo.cpp`), que
  usan `recursos/fuente/ARCO_juego.ttf`. **No usar `DrawText`/`MeasureText`
  directo**: saldrían con la fuente de fábrica y mal centrados.
- `ARCO_juego.ttf` es `ARCO.ttf` con una tabla de caracteres Unicode agregada
  (con fontTools). La original solo trae la tabla "symbol" de Windows y raylib
  la rechaza ("Failed to process TTF font data"). La original está en
  `arte_original/ARCO_original.ttf`. La fuente es de puras mayúsculas.
- Color del texto: `#552000` (`COLOR_TEXTO_MADERA` en `Tema.hpp`). Única
  excepción: en Configuración lo que va directo sobre la madera oscura usa
  `COLOR_TEXTO_CLARO`.

---

## Pruebas

`Partida.cpp`, `Tablero.cpp`, `Dificultad.cpp`, `Aleatorio.cpp` y
`TablaPuntajes.cpp` **no incluyen raylib**, así que se pueden compilar y correr solos:

```
g++ -std=c++11 -Wall -Wextra -o prueba prueba.cpp \
    Partida.cpp Tablero.cpp Dificultad.cpp Aleatorio.cpp ConfigPartida.cpp \n    TablaPuntajes.cpp
```

Ya se validó así: pozos/vidas/tiempos por nivel, nombre por defecto, tope de 2
objetos en los tres niveles, escalones de 0.2 s cada 10 apariciones hasta el
piso, probabilidad del dorado (15/50/100 %, nunca 30 sin dorado, ~15 % en
promedio), +1 topo y +1 dorado, bomba (−2, −1 vida, racha a 0), aplastado que
ocupa el pozo 0.5 s y no se puede volver a golpear, 3 escapes no seguidos = −1
vida (bombas que se esconden no cuentan), puntaje que no va a negativos y fin de
partida. De la tabla de puntajes: orden por puntos, desempate por racha y luego
por antigüedad, tope de 10 por nivel, guardar y leer el archivo (con nombres
con espacios), archivo inexistente y renglones basura.
**Si se tocan las reglas, conviene rehacer esa prueba.**

---

## Lo que sigue pendiente

- `Opciones.cpp` sigue como pantalla de relleno con `dibujarPantallaPendiente()`. `Escena_opciones` ni
  siquiera tiene entrada desde el menú.
- `Monticulo.hpp` está reservado a propósito y **vacío**. Se decidió no meter un
  montículo porque con 10 pozos revisarlos todos cada fotograma son 10
  comparaciones; el archivo tiene la explicación. Si el profe lo pide, ahí va.
- `recursos/imagenes/icono_sonido.png` quedó sin usar (lo reemplazó el engrane
  de ajustes).
- Los dos `sfx_*.mp3` son sonidos generados de relleno, se pueden reemplazar por
  otros mejores sin tocar código.
