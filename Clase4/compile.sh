#!/bin/bash
# =============================================================================
# compile.sh — Script de compilación del proyecto
#
# BUENA PRÁCTICA: Un script de compilación centraliza las opciones del
# compilador. El equipo siempre compila igual, evitando errores por flags
# olvidados o versiones distintas de comandos.
#
# USO: ./compile.sh
# =============================================================================

# Colores para mensajes en consola
VERDE="\033[0;32m"
ROJO="\033[0;31m"
AMARILLO="\033[1;33m"
AZUL="\033[0;34m"
RESET="\033[0m"

echo -e "${AZUL}"
echo "  ╔══════════════════════════════════════════╗"
echo "  ║   COMPILANDO: Sistema Académico LFP      ║"
echo "  ║   Lenguajes Formales y de Programación   ║"
echo "  ╚══════════════════════════════════════════╝"
echo -e "${RESET}"

# ─────────────────────────────────────────────────────────────────────────────
# CONFIGURACIÓN
# ─────────────────────────────────────────────────────────────────────────────
COMPILADOR="g++"
ESTANDAR="-std=c++17"          # C++17 para structured bindings y otras features
WARNINGS="-Wall -Wextra"        # Activar todos los warnings (buena práctica)
OPTIMIZACION="-O2"              # Optimización moderada para release
DEBUG="-g"                      # Información de debug (quitar en producción)
SALIDA="build/sistema_academico"

# Archivos fuente (BUENA PRÁCTICA: lista explícita, no usar wildcards en producción)
FUENTES="src/main.cpp \
         src/services/FileReader.cpp \
         src/services/Statistics.cpp \
         src/services/ReportGenerator.cpp"

# ─────────────────────────────────────────────────────────────────────────────
# VERIFICACIONES PREVIAS
# ─────────────────────────────────────────────────────────────────────────────

# Verificar que g++ esté instalado
if ! command -v $COMPILADOR &> /dev/null; then
    echo -e "${ROJO}  [ERROR] No se encontró '$COMPILADOR'. Instálelo con:${RESET}"
    echo "          sudo apt install g++    (Ubuntu/Debian)"
    echo "          brew install gcc        (macOS)"
    exit 1
fi

echo -e "${AMARILLO}  [INFO] Compilador: $(${COMPILADOR} --version | head -1)${RESET}"

# Crear directorio build si no existe
mkdir -p build
mkdir -p reportes

echo -e "${AMARILLO}  [INFO] Compilando archivos fuente...${RESET}"

# ─────────────────────────────────────────────────────────────────────────────
# COMPILACIÓN
# ─────────────────────────────────────────────────────────────────────────────
$COMPILADOR $ESTANDAR $WARNINGS $DEBUG $FUENTES -o $SALIDA

# Verificar si la compilación fue exitosa
# $? contiene el código de retorno del último comando (0 = éxito)
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${VERDE}  ✔ Compilación exitosa!${RESET}"
    echo -e "${VERDE}  ✔ Ejecutable: ${SALIDA}${RESET}"
    echo ""
    echo -e "${AZUL}  Para ejecutar el programa use:${RESET}"
    echo "     ./run.sh"
    echo "     — o bien —"
    echo "     ./${SALIDA}"
    echo ""
else
    echo ""
    echo -e "${ROJO}  ✘ Error en la compilación. Revise los mensajes anteriores.${RESET}"
    echo ""
    exit 1
fi