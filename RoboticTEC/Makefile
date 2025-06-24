# Makefile para el proyecto RoboticTEC
CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Wextra -g -O2
LDFLAGS = 

# Directorios
SRCDIR = .
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

# Archivos fuente
BIBLIOTECA_SRC = biblioteca.c
SERVIDOR_SRC = server.c
CLIENTE_SRC = client.c
NODO_SRC = nodo.c

# Archivos objeto
BIBLIOTECA_OBJ = $(OBJDIR)/biblioteca.o
SERVIDOR_OBJ = $(OBJDIR)/server.o
CLIENTE_OBJ = $(OBJDIR)/client.o
NODO_OBJ = $(OBJDIR)/nodo.o

# Ejecutables
SERVIDOR_BIN = $(BINDIR)/server
CLIENTE_BIN = $(BINDIR)/client
NODO_BIN = $(BINDIR)/nodo
BIBLIOTECA_LIB = $(LIBDIR)/libmano.a

# Regla por defecto
all: directories $(BIBLIOTECA_LIB) $(SERVIDOR_BIN) $(CLIENTE_BIN) $(NODO_BIN)
	@echo "Compilación completada exitosamente!"
	@echo "Ejecutables disponibles:"
	@echo "  - Servidor: $(SERVIDOR_BIN)"
	@echo "  - Cliente:  $(CLIENTE_BIN)"
	@echo "  - Nodo MPI: $(NODO_BIN)"
	@echo "  - Biblioteca: $(BIBLIOTECA_LIB)"

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJDIR) $(BINDIR) $(LIBDIR) archivos

# Compilar biblioteca estática
$(BIBLIOTECA_LIB): $(BIBLIOTECA_OBJ)
	@echo "Creando biblioteca estática..."
	@ar rcs $@ $^
	@echo "Biblioteca creada: $@"

# Compilar servidor
$(SERVIDOR_BIN): $(SERVIDOR_OBJ) $(BIBLIOTECA_LIB)
	@echo "Compilando servidor..."
	$(CC) $(CFLAGS) -o $@ $< -L$(LIBDIR) -lmano $(LDFLAGS)

# Compilar cliente
$(CLIENTE_BIN): $(CLIENTE_OBJ)
	@echo "Compilando cliente..."
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Compilar nodo MPI
$(NODO_BIN): $(NODO_OBJ)
	@echo "Compilando nodo MPI..."
	$(MPICC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Reglas para objetos individuales
$(OBJDIR)/biblioteca.o: $(BIBLIOTECA_SRC) biblioteca.h
	@echo "Compilando biblioteca.c..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/server.o: $(SERVIDOR_SRC) biblioteca.h
	@echo "Compilando server.c..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/client.o: $(CLIENTE_SRC)
	@echo "Compilando client.c..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/nodo.o: $(NODO_SRC)
	@echo "Compilando nodo.c..."
	$(MPICC) $(CFLAGS) -c $< -o $@

# Instalar (copiar ejecutables al directorio actual para facilitar uso)
install: all
	@echo "Instalando ejecutables..."
	@cp $(SERVIDOR_BIN) ./server
	@cp $(CLIENTE_BIN) ./client  
	@cp $(NODO_BIN) ./nodo
	@echo "Ejecutables instalados en el directorio actual."

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	@rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)
	@rm -f server client nodo
	@rm -f archivos/archivo_cifrado.txt archivos/archivo_descifrado.txt
	@echo "Limpieza completada."

# Limpiar todo incluyendo directorios
distclean: clean
	@rm -rf archivos
	@echo "Limpieza completa realizada."

# Probar compilación y dependencias
test-deps:
	@echo "Verificando dependencias..."
	@which gcc > /dev/null || (echo "ERROR: gcc no encontrado" && exit 1)
	@which mpicc > /dev/null || (echo "ERROR: mpicc no encontrado. Instalar OpenMPI" && exit 1)
	@which mpirun > /dev/null || (echo "ERROR: mpirun no encontrado. Instalar OpenMPI" && exit 1)
	@echo "Todas las dependencias están disponibles."

# Ejecutar una prueba completa del sistema
test-system: all install
	@echo "=== EJECUTANDO PRUEBA DEL SISTEMA ==="
	@echo "Creando archivo de prueba..."
	@echo "Esta es una prueba del sistema. Esta palabra se repite varias veces. Sistema sistema sistema prueba." > test_input.txt
	@echo "Iniciando servidor en segundo plano..."
	@./server &
	@SERVER_PID=$$!; \
	sleep 2; \
	echo "Enviando archivo de prueba..."; \
	./client test_input.txt 127.0.0.1; \
	sleep 1; \
	kill $$SERVER_PID 2>/dev/null || true; \
	echo "Prueba completada."
	@rm -f test_input.txt archivo_cifrado.txt

# Mostrar ayuda
help:
	@echo "Makefile para el proyecto RoboticTEC"
	@echo ""
	@echo "Objetivos disponibles:"
	@echo "  all        - Compilar todo el proyecto"
	@echo "  install    - Compilar e instalar ejecutables"
	@echo "  clean      - Limpiar archivos generados"
	@echo "  distclean  - Limpieza completa"
	@echo "  test-deps  - Verificar dependencias"
	@echo "  test-system- Ejecutar prueba del sistema"
	@echo "  help       - Mostrar esta ayuda"
	@echo ""
	@echo "Dependencias requeridas:"
	@echo "  - gcc (compilador C)"
	@echo "  - mpicc/mpirun (OpenMPI)"
	@echo ""
	@echo "Uso típico:"
	@echo "  make test-deps    # Verificar dependencias"
	@echo "  make install      # Compilar e instalar"
	@echo "  ./server          # Ejecutar servidor"
	@echo "  ./client archivo.txt 127.0.0.1  # Enviar archivo"

.PHONY: all directories install clean distclean test-deps test-system help
