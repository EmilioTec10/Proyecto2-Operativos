#!/bin/bash

# Script de instalación rápida para archivos corregidos
# RoboticTEC - Proyecto de Sistemas Operativos

set -e  # Salir si hay algún error

COLOR_GREEN='\033[0;32m'
COLOR_YELLOW='\033[1;33m'
COLOR_RED='\033[0;31m'
COLOR_BLUE='\033[0;34m'
COLOR_NC='\033[0m'

echo -e "${COLOR_BLUE}=== INSTALACIÓN RÁPIDA - ARCHIVOS CORREGIDOS ===${COLOR_NC}"
echo ""

# Verificar que estamos en el directorio correcto
if [ ! -f "biblioteca.h" ]; then
    echo -e "${COLOR_RED}Error: No se encontró biblioteca.h${COLOR_NC}"
    echo "Asegúrate de estar en el directorio del proyecto"
    exit 1
fi

# Paso 1: Backup de archivos originales
echo -e "${COLOR_YELLOW}1. Creando backups de archivos originales...${COLOR_NC}"
for file in server.c client.c nodo.c; do
    if [ -f "$file" ]; then
        cp "$file" "${file}.backup"
        echo "   ✓ Backup creado: ${file}.backup"
    fi
done

# Paso 2: Verificar que los archivos _fixed existen
echo -e "${COLOR_YELLOW}2. Verificando archivos corregidos...${COLOR_NC}"
missing_files=""

if [ ! -f "server_fixed.c" ]; then
    missing_files="$missing_files server_fixed.c"
fi

if [ ! -f "client_fixed.c" ]; then
    missing_files="$missing_files client_fixed.c"
fi

if [ ! -f "nodo_fixed.c" ]; then
    missing_files="$missing_files nodo_fixed.c"
fi

if [ -n "$missing_files" ]; then
    echo -e "${COLOR_RED}Error: Faltan archivos corregidos:${COLOR_NC}"
    for file in $missing_files; do
        echo "   ✗ $file"
    done
    echo ""
    echo "Por favor, copia el contenido de los artifacts a estos archivos."
    echo "Los artifacts están en la respuesta de Claude con los archivos corregidos."
    exit 1
fi

echo "   ✓ Todos los archivos corregidos están presentes"

# Paso 3: Crear/actualizar Makefile
echo -e "${COLOR_YELLOW}3. Actualizando Makefile...${COLOR_NC}"

cat > Makefile << 'EOF'
# Makefile para el proyecto RoboticTEC - VERSION CORREGIDA
CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Wextra -g -O2
LDFLAGS = 

# Directorios
SRCDIR = .
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

# Archivos fuente (versiones corregidas)
BIBLIOTECA_SRC = biblioteca.c
SERVIDOR_SRC = server_fixed.c
CLIENTE_SRC = client_fixed.c
NODO_SRC = nodo_fixed.c

# Archivos objeto
BIBLIOTECA_OBJ = $(OBJDIR)/biblioteca.o
SERVIDOR_OBJ = $(OBJDIR)/server_fixed.o
CLIENTE_OBJ = $(OBJDIR)/client_fixed.o
NODO_OBJ = $(OBJDIR)/nodo_fixed.o

# Ejecutables
SERVIDOR_BIN = $(BINDIR)/server
CLIENTE_BIN = $(BINDIR)/client
NODO_BIN = $(BINDIR)/nodo
BIBLIOTECA_LIB = $(LIBDIR)/libmano.a

all: directories $(BIBLIOTECA_LIB) $(SERVIDOR_BIN) $(CLIENTE_BIN) $(NODO_BIN)
	@echo "Compilación completada exitosamente!"

directories:
	@mkdir -p $(OBJDIR) $(BINDIR) $(LIBDIR) archivos

$(BIBLIOTECA_LIB): $(BIBLIOTECA_OBJ)
	@echo "Creando biblioteca estática..."
	@ar rcs $@ $^

$(SERVIDOR_BIN): $(SERVIDOR_OBJ) $(BIBLIOTECA_LIB)
	@echo "Compilando servidor..."
	$(CC) $(CFLAGS) -o $@ $< -L$(LIBDIR) -lmano $(LDFLAGS)

$(CLIENTE_BIN): $(CLIENTE_OBJ)
	@echo "Compilando cliente..."
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(NODO_BIN): $(NODO_OBJ)
	@echo "Compilando nodo MPI..."
	$(MPICC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(OBJDIR)/biblioteca.o: $(BIBLIOTECA_SRC) biblioteca.h
	@echo "Compilando biblioteca.c..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/server_fixed.o: $(SERVIDOR_SRC) biblioteca.h
	@echo "Compilando server_fixed.c..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/client_fixed.o: $(CLIENTE_SRC)
	@echo "Compilando client_fixed.c..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/nodo_fixed.o: $(NODO_SRC)
	@echo "Compilando nodo_fixed.c..."
	$(MPICC) $(CFLAGS) -c $< -o $@

install: all
	@echo "Instalando ejecutables..."
	@cp $(SERVIDOR_BIN) ./server
	@cp $(CLIENTE_BIN) ./client  
	@cp $(NODO_BIN) ./nodo
	@echo "Ejecutables instalados."

clean:
	@echo "Limpiando archivos generados..."
	@rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)
	@rm -f server client nodo
	@echo "Limpieza completada."

test-quick: install
	@echo "=== PRUEBA RÁPIDA ==="
	@echo "Creando archivo de prueba..."
	@echo "esta es una prueba prueba del sistema sistema sistema" > test.txt
	@echo "Iniciando servidor en background..."
	@timeout 10 ./server &
	@sleep 1
	@echo "Enviando archivo..."
	@timeout 5 ./client test.txt 127.0.0.1 || true
	@sleep 1
	@killall server 2>/dev/null || true
	@rm -f test.txt archivo_cifrado.txt
	@echo "Prueba rápida completada."

.PHONY: all directories install clean test-quick
EOF

echo "   ✓ Makefile actualizado"

# Paso 4: Compilar
echo -e "${COLOR_YELLOW}4. Compilando proyecto...${COLOR_NC}"
if make clean && make all; then
    echo -e "   ${COLOR_GREEN}✓ Compilación exitosa${COLOR_NC}"
else
    echo -e "   ${COLOR_RED}✗ Error en compilación${COLOR_NC}"
    exit 1
fi

# Paso 5: Instalar ejecutables
echo -e "${COLOR_YELLOW}5. Instalando ejecutables...${COLOR_NC}"
if make install; then
    echo -e "   ${COLOR_GREEN}✓ Instalación exitosa${COLOR_NC}"
else
    echo -e "   ${COLOR_RED}✗ Error en instalación${COLOR_NC}"
    exit 1
fi

# Paso 6: Verificar ejecutables
echo -e "${COLOR_YELLOW}6. Verificando ejecutables...${COLOR_NC}"
for exec in server client nodo; do
    if [ -x "./$exec" ]; then
        echo "   ✓ $exec - OK"
    else
        echo "   ✗ $exec - NO ENCONTRADO"
    fi
done

# Paso 7: Información final
echo ""
echo -e "${COLOR_GREEN}=== INSTALACIÓN COMPLETADA ===${COLOR_NC}"
echo ""
echo "Archivos disponibles:"
echo "  • ./server      - Servidor principal"
echo "  • ./client      - Cliente para envío de archivos" 
echo "  • ./nodo        - Procesador MPI distribuido"
echo ""
echo "Para probar el sistema:"
echo "  1. Terminal 1: ./server"
echo "  2. Terminal 2: ./client mi_archivo.txt 127.0.0.1"
echo ""
echo "Para prueba rápida:"
echo "  make test-quick"
echo ""
echo -e "${COLOR_BLUE}¡Sistema listo para usar!${COLOR_NC}"
