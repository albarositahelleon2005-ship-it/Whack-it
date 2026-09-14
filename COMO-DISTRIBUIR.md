# Cómo convertir Whack it en un juego que cualquiera pueda abrir

Todo lo de este documento ya quedó configurado en el proyecto. Lo único que
falta lo tienes que hacer tú, porque **el ejecutable se tiene que compilar en
Windows**, en tu máquina.

---

## Los 3 pasos

### 1. Compilar en modo Release

En Code::Blocks, arriba en la barra hay una lista que dice **Debug**.
Cámbiala a **Release** y compila (`Build`, o `Ctrl+F9`).

Eso deja el juego en `bin\Release\WhackIt.exe`.

> Si es la primera vez después de este cambio, usa **Build > Rebuild** para que
> vuelva a compilar todo desde cero.

### 2. Empaquetar

Doble clic a **`empaquetar.bat`**.

Arma la carpeta `Distribuir\WhackIt` con el `.exe`, la carpeta `recursos` y el
`LEEME.txt`, y además deja un `Distribuir\WhackIt.zip` listo para mandar por
WhatsApp, Drive o correo.

### 3. (Opcional) Hacer un instalador de verdad

Si quieres que se instale como cualquier programa —con su acceso directo en el
escritorio y su entrada en "Agregar o quitar programas"— instala
[Inno Setup](https://jrsoftware.org/isdl.php), abre **`instalador.iss`** y
presiona `F9`. Te genera un solo archivo `WhackIt-instalador.exe`.

Para un trabajo escolar el ZIP del paso 2 basta y sobra. El instalador se ve
más profesional si lo vas a presentar o subir a algún lado.

---

## Qué se cambió y por qué

| Cambio | Para qué |
|---|---|
| `Release` ahora es aplicación de ventana (`-mwindows`) | Ya no sale la **ventana negra de consola** detrás del juego |
| `-static` en el enlazador | El `.exe` ya no depende de las DLL de MinGW. Sin esto funciona en tu compu pero en la de tus amigos truena con *"falta libstdc++-6.dll"* |
| `icono.rc` + `icono.ico` | El `.exe` tiene **ícono propio** (el gatito saliendo del pozo) en el Explorador y en la barra de tareas, y datos en *Propiedades > Detalles* |
| `ChangeDirectory(GetApplicationDirectory())` en `main.cpp` | El juego encuentra su carpeta `recursos` aunque lo abran desde un acceso directo o desde el menú inicio |
| Pantalla de "Cargando..." | Los GIFs tardan un par de segundos en prepararse; antes la ventana se quedaba en blanco y Windows podía marcarla como *"no responde"* |
| `LEEME.txt` | Instrucciones para quien nunca ha visto el juego |

El objetivo **Debug sigue igual** (con consola), para que puedas seguir viendo
los mensajes de raylib mientras programas.

---

## Lo que hay que saber al repartirlo

**El `.exe` no va solo.** Necesita la carpeta `recursos` a un lado. Si alguien
saca nada más el `WhackIt.exe`, va a abrir sin imágenes ni música. Por eso se
reparte la **carpeta completa** (o el instalador).

**Windows va a desconfiar la primera vez.** Va a salir una pantalla azul que
dice *"Windows protegió tu PC"*. Es normal: pasa con todo programa nuevo que no
está firmado digitalmente (una firma cuesta varios cientos de dólares al año).
Se resuelve con **Más información > Ejecutar de todas formas**, y ya está
explicado en el `LEEME.txt`.

**Es de 64 bits**, igual que tu compilador. Cualquier Windows de los últimos
diez años lo corre.

---

## Si algo falla

**`-static` da error al enlazar.**
Algunos MinGW no traen todas las librerías estáticas. En Code::Blocks:
`Project > Build options... > Release > Linker settings`, quita `-static` y pon
en su lugar estas dos líneas:

```
-static-libgcc
-static-libstdc++
```

Cubren el 99% de los casos.

**El ícono no aparece.**
Windows guarda los íconos en caché y a veces sigue mostrando el viejo. Copia el
`.exe` a otra carpeta para comprobarlo. Si de plano no sale, revisa que
`icono.rc` aparezca en el árbol del proyecto en Code::Blocks; si no, agrégalo
con `Project > Add files...`.

**Code::Blocks marca error al compilar `icono.rc`.**
Algunas versiones viejas de `windres` son quisquillosas con el bloque
`VERSIONINFO` (el de los datos de *Propiedades*). Ese bloque es puro adorno:
abre `icono.rc` y borra todo lo que está debajo de la línea
`GLFW_ICON ICON "icono.ico"`. El ícono sigue funcionando igual.

**El juego abre y se cierra solo.**
Casi siempre es que falta la carpeta `recursos`. Compila en **Debug** y ábrelo
desde la consola: ahí raylib te dice exactamente qué archivo no encontró.

---

## Archivos nuevos

```
icono.ico            el ícono, ya listo para el .exe
icono.png            el mismo ícono en grande, por si quieres editarlo
icono.rc             le dice al compilador que meta el ícono dentro del .exe
empaquetar.bat       arma la carpeta y el zip para repartir
instalador.iss       guion para Inno Setup (opcional)
LEEME.txt            instrucciones para los jugadores
COMO-DISTRIBUIR.md   este documento
```
