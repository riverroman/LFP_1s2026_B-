#!/bin/bash
# =============================================================================
# clean.sh — Limpia los archivos compilados y reportes generados
#
# BUENA PRÁCTICA: Siempre tener un script clean para mantener el repo limpio.
# Los binarios y reportes NO se suben a Git (se agregan al .gitignore).
# =============================================================================

ROJO="\033[0;31m"
VERDE="\033[0;32m"
RESET="\033[0m"

echo -e "${ROJO}  Limpiando archivos generados...${RESET}"

# Eliminar ejecutables compilados
rm -rf build/
echo "  ✔ Carpeta build/ eliminada"

# Eliminar reportes HTML (son generados, no deben versionarse)
rm -f reportes/*.html
echo "  ✔ Reportes HTML eliminados"

echo ""
echo -e "${VERDE}  Limpieza completada. Use ./compile.sh para recompilar.${RESET}"
echo ""