; ===================================================================
;  instalador.iss - Guion para Inno Setup (gratis: jrsoftware.org/isdl.php)
;
;  Genera UN solo archivo "WhackIt-instalador.exe" que instala el juego,
;  crea el acceso directo en el escritorio y aparece en "Agregar o quitar
;  programas", como cualquier programa normal de Windows.
;
;  Como usarlo:
;    1. Compila el juego en Code::Blocks con el objetivo "Release".
;    2. Instala Inno Setup.
;    3. Doble clic a este archivo y presiona Build > Compile (o F9).
;    4. El instalador queda en la carpeta Distribuir.
; ===================================================================

#define MiNombre    "Whack it"
#define MiVersion   "1.0"
#define MiEjecutable "WhackIt.exe"

[Setup]
AppName={#MiNombre}
AppVersion={#MiVersion}
AppPublisher=Universidad de Sonora
DefaultDirName={autopf}\{#MiNombre}
DefaultGroupName={#MiNombre}
UninstallDisplayIcon={app}\{#MiEjecutable}
OutputDir=Distribuir
OutputBaseFilename=WhackIt-instalador
SetupIconFile=icono.ico
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
DisableProgramGroupPage=yes
; El juego es de 64 bits, asi que se instala en "Archivos de programa" de 64.
ArchitecturesInstallIn64BitMode=x64compatible
ArchitecturesAllowed=x64compatible

[Languages]
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "escritorio"; Description: "Crear un acceso directo en el escritorio"; GroupDescription: "Extras:"

[Files]
Source: "bin\Release\{#MiEjecutable}"; DestDir: "{app}"; Flags: ignoreversion
Source: "recursos\*";                  DestDir: "{app}\recursos"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "LEEME.txt";                   DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\{#MiNombre}";      Filename: "{app}\{#MiEjecutable}"
Name: "{autodesktop}\{#MiNombre}"; Filename: "{app}\{#MiEjecutable}"; Tasks: escritorio

[Run]
Filename: "{app}\{#MiEjecutable}"; Description: "Jugar ahora"; Flags: nowait postinstall skipifsilent
