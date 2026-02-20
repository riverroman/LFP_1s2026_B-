#!/bin/bash
# =============================================================================
# run.sh — Script de ejecución del proyecto
#
# Verifica que el ejecutable exista antes de correrlo.
# Si no existe, invoca el script de compilación primero.
#
# USO: ./run.sh
# =============================================================================

VERDE="\033[0;32m"
ROJO="\033[0;31m"
AMARILLO="\033[1;33m"
RESET="\033[0m"

EJECUTABLE="build/sistema_academico"

echo -e "${AMARILLO}"
echo "  ══════════════════════════════════════════"
echo "   Sistema de Análisis Académico – LFP 2026"
echo "  ══════════════════════════════════════════"
echo -e "${RESET}"

# Verificar si el ejecutable existe
if [ ! -f "$EJECUTABLE" ]; then
    echo -e "${AMARILLO}  [INFO] Ejecutable no encontrado. Compilando primero...${RESET}"
    echo ""
    bash compile.sh

    # Si la compilación falló, salir
    if [ $? -ne 0 ]; then
        echo -e "${ROJO}  [ERROR] No se pudo compilar el proyecto.${RESET}"
        exit 1
    fi
fi

echo -e "${VERDE}  [OK] Iniciando el sistema...${RESET}"
echo ""

# Ejecutar el programa desde la raíz del proyecto
# (importante para que las rutas relativas funcionen correctamente)
./$EJECUTABLE