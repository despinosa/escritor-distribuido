#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "escritor_ipc"
#define TAM_DGRAMA 100

Servidor::Servidor() {
    struct sockaddr_un local;
    size_t tam;
    descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(descriptor < 0) {
        throw "error al crear socket";
    }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    tam = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(descriptor, (struct sockaddr *)&local, tam) < 0) {
        throw "error al registrar";
    }
}

Servidor::~Servidor() {
    close(descriptor);
}

void Servidor::recibir(size_t nbytes, char *buffer) {
    if(recv(descriptor, buffer, nbytes, 0) < 0) {
        throw "error al recibir";
    }
}

int Servidor::recibir_n() {
    if(recv(descriptor, &n, sizeof(int), 0) < 0) {
        throw "error al recibir n";
    }
    return n;
}

void Servidor::palabra_aleatoria(char *buffer, char *palabra) {
    palabra = strtok(buffer, " ");
    while(palabra != 0) {
        if(rand() % 10 < 2) {
            return;
        }
        palabra = strtok(buffer, " ");
    }
    palabra_aleatoria(buffer, palabra);
}

void Servidor::ejecutar(unsigned int repeticiones) {
    char filename[42], palabra[42], buffer[TAM_DGRAMA+1];
    for (int i = 0; i < repeticiones; ++i) {
        // descriptor = aceptar();
        recibir_n();
        for(int i = 0; i < n; ++i) {
            sprintf(filename, "%d.txt", i);
            archivos.push_back(Archivo(filename, O_WRONLY | O_CREAT,
                                       O_WRONLY | S_IWUSR));
            recibir(TAM_DGRAMA, buffer);
            palabra_aleatoria(buffer, palabra);
            archivos.back().escribe(palabra, strlen(palabra));
            archivos.back().cerrar();
            close(descriptor);
        }
    }
}

int main(int argc, char const *argv[]) {
    if(argc != 2) {
        printf("forma de uso: %s <repeticiones>\n", argv[0]);
        exit(-1);
    }
    try {
        Servidor servidor;
        servidor.ejecutar(atoi(argv[1]));
    } catch(const char *msg) {
        perror(msg);
        printf("%s\n", msg);
        exit(-1);
    }
    return 0;
}

