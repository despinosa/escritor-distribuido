#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

#define RUTA_SERV "escritor_servidor"
#define RUTA_CLI "escritor_cliente"
#define TAM_DGRAMA 100

Servidor::Servidor() {
    size_t tam;
    srand(time(NULL));
    descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(descriptor < 0) {
        throw "error al crear socket";
    }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, RUTA_SERV);
    unlink(local.sun_path);
    tam = strlen(local.sun_path) + sizeof(local.sun_family);
    if(bind(descriptor, (struct sockaddr*) &local, tam) < 0) {
        throw "error al registrar";
    }
    remota.sun_family = AF_UNIX;
    strcpy(remota.sun_path, RUTA_CLI);
}

Servidor::~Servidor() {
    close(descriptor);
}

char *Servidor::recibir(size_t nbytes) {
    char *buffer;
    socklen_t tam;
    buffer = (char*) malloc(nbytes+1);
    tam = strlen(remota.sun_path) + sizeof(remota.sun_family);
    if(recvfrom(descriptor, buffer, nbytes, 0, (struct sockaddr*) &remota,
                &tam) < 0) {
        throw "error al recibir";
    }
    return buffer;
}

int Servidor::recibir_n() {
    socklen_t tam;
    tam = strlen(remota.sun_path) + sizeof(remota.sun_family);
    if(recvfrom(descriptor, &n, sizeof(int), 0, (struct sockaddr*) &remota,
                &tam) < 0) {
        throw "error al recibir n";
    }
    return n;
}

char *Servidor::palabra_aleatoria(char *buffer) {
    char *palabra;
    palabra = strtok(buffer, " \n\t.,;:");
    while(palabra != NULL) {
        if(rand() % 10 == 0) {
            return palabra;
        }
        palabra = strtok(NULL, " \n\t.,;:");
    }
    return palabra_aleatoria(buffer);
}

void Servidor::ejecutar(unsigned int repeticiones) {
    char filename[42], *palabra, *buffer;
    for (int i = 0; i < repeticiones; ++i) {
        recibir_n();
        for(int i = 0; i < n; ++i) {
            sprintf(filename, "%d.txt", i);
            archivos.push_back(Archivo(filename, O_WRONLY | O_CREAT,
                                       777));
            buffer = recibir(TAM_DGRAMA);
            palabra = palabra_aleatoria(buffer);
            printf("%s\n", palabra);
            archivos.back().escribe((const char*) palabra, strlen(palabra));
            archivos.back().cerrar();
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
        // printf("%s\n", msg);
        exit(-1);
    }
    return 0;
}

