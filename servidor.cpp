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
    mi_descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(mi_descriptor < 0) {
        throw "error al crear socket";
    }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    tam = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(mi_descriptor, (struct sockaddr *)&local, tam) < 0) {
        throw "error al registrar";
    }
    if (listen(mi_descriptor, 5) < 0) {
        throw "error al escuchar";
    }
}

Servidor::~Servidor() {
    close(mi_descriptor);
}

int Servidor::aceptar() {
    struct sockaddr_un remoto;
    socklen_t tam;
    tam = sizeof(remoto);
    otro_descriptor = accept(mi_descriptor, (struct sockaddr*) &remoto, &tam);
    if (otro_descriptor < 0) {
        throw "error al aceptar";
    }
    return otro_descriptor;
}

void Servidor::recibir(size_t nbytes, char *buffer) {
    if(recv(otro_descriptor, buffer, nbytes, 0) < 0) {
        throw "error al recibir";
    }
}

int Servidor::recibir_n() {
    if(recv(otro_descriptor, &n, sizeof(int), 0) < 0) {
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
        otro_descriptor = aceptar();
        recibir_n();
        for(int i = 0; i < n; ++i) {
            sprintf(filename, "%d.txt", i);
            archivos.push_back(Archivo(filename, O_RDONLY));
            recibir(TAM_DGRAMA, buffer);
            palabra_aleatoria(buffer, palabra);
            archivos.back().escribe(palabra, strlen(palabra));
            archivos.back().cerrar();
            close(otro_descriptor);
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

