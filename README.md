# Proyecto2-Operativos
# Instalar OpenMPI
cd RoboticTEC
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev

# Verificar Instalacion
mpicc --version
mpirun --version

# Ejecutar nodo
mpicc nodo.c -o nodo

# Compilar Servidor
gcc server.c -o server
./server



# En una terminal (servidor MPI: 4 procesos en localhost)
mpirun -np 4 ./server 5000

# En otra terminal (cliente)
gcc -Wall -Wextra -std=c11 -o client client.c
./client archivos/el_quijote.txt 127.0.0.1

