# Proyecto2-Operativos



# Instalar OpenMPI
cd RoboticTEC
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev

# Verificar Instalacion
mpicc --version
mpirun --version

# Compilar Servidor
mpicc -Wall -Wextra -pedantic -std=c11 -o server server.c

# En una terminal (servidor MPI: 4 procesos en localhost)
mpirun -np 4 ./server 5000

# En otra terminal (cliente)
gcc -Wall -Wextra -std=c11 -o client client.c
./client mensaje.txt
