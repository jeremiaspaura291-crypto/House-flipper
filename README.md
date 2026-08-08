# House Flipper 2D

**House Flipper 2D** es un prototipo jugable de supervivencia y movimiento escrito en C++17, integrado en este repositorio para validar el flujo de juego nativo y la generación Android. El jugador controla un trabajador dentro de una casa estilizada, recoge objetivos de trabajo y evita obstáculos móviles mientras aumenta su puntuación.

## Controles

En escritorio se puede jugar con `WASD` o las flechas. `Enter` o `Espacio` comienza la partida y `R` reinicia después de perder. En Android, las zonas táctiles izquierda, derecha, superior e inferior controlan el movimiento correspondiente.

## Arquitectura

La lógica está separada de la presentación. `src/Game.cpp` contiene el modelo determinista de la partida; `src/main.cpp` contiene la presentación 2D con raylib y la adaptación de teclado/táctil. `tests/GameTests.cpp` comprueba el arranque, movimiento y reinicio. CMake genera el ejecutable de escritorio o la biblioteca nativa Android según la plataforma.

## Compilación de escritorio

Se requiere CMake 3.20 o superior y un compilador C++17:

```bash
cmake -S . -B build -DNEON_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
./build/house_flipper
```

La primera configuración descarga raylib desde GitHub mediante `FetchContent`. El binario necesita una sesión gráfica para ejecutarse.

## APK Android

La compilación final del APK se realiza exclusivamente en **GitHub Actions**. El workflow `.github/workflows/android.yml` instala JDK 17, Android SDK, NDK 26.3.11579264 y CMake 3.22.1; ejecuta las pruebas de lógica y genera `app-debug.apk` para `arm64-v8a`.

Para compilarlo, abrir la pestaña **Actions** del repositorio, seleccionar **House Flipper Android APK** y ejecutar el workflow manualmente. El archivo aparecerá como artefacto descargable con el nombre `house-flipper-android-debug-N`.

Los builds release firmados no están habilitados todavía porque requieren un keystore y secretos de GitHub. No se incluyen credenciales en el repositorio.

## Estado

La compilación de escritorio y las pruebas deterministas se verifican antes de publicar los cambios. La compilación Android se ejecuta en un runner limpio de GitHub Actions, que es la fuente del APK final.
