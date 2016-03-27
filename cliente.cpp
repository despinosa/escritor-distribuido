#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define RUTA_SERV "escritor_servidor"
#define RUTA_CLI "escritor_cliente"
#define TAM_DGRAMA 100

Cliente::Cliente(const char *filename, unsigned int n) :
        archivo(filename, O_RDONLY) {
    size_t tam;
    this->n = n;
    descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(descriptor < 0) {
        throw "error al crear socket";
    }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, RUTA_CLI);
    unlink(local.sun_path);
    tam = strlen(local.sun_path) + sizeof(local.sun_family);
    if(bind(descriptor, (struct sockaddr *)&local, tam) < 0) {
        throw "error al registrar";
    }
    remota.sun_family = AF_UNIX;
    strcpy(remota.sun_path, RUTA_SERV);
}

Cliente::~Cliente() {
    archivo.cerrar();
    // close(descriptor);
}

void Cliente::conectar() {
    int len;
    remota.sun_family = AF_UNIX;
    strcpy(remota.sun_path, RUTA_SERV);
    len = strlen(remota.sun_path) + sizeof(remota.sun_family);
    if(connect(descriptor, (struct sockaddr*) &remota, len) < 0) {
        throw "error al conectar";
    }
}

void Cliente::enviar(char *str) {
    socklen_t tam;
    tam = strlen(remota.sun_path) + sizeof(remota.sun_family);
    if(sendto(descriptor, str, strlen(str), 0, (struct sockaddr*) &remota,
              tam) < 0) {
        throw "error al enviar";
    }
}

void Cliente::enviar_n() {
    socklen_t tam;
    tam = strlen(remota.sun_path) + sizeof(remota.sun_family);
    if(sendto(descriptor, &n, sizeof(int), 0, (struct sockaddr*) &remota,
              tam) < 0) {
        throw "error al enviar n";
    }
}

void Cliente::ejecutar() {
    size_t leido;
    // conectar();
    enviar_n();
    for(int i = 0; i < n; ++i) {
        leido = archivo.lee(TAM_DGRAMA);
        if(leido < TAM_DGRAMA) break;
        enviar(archivo.get_contenido());
    }
}

int main(int argc, char const *argv[]) {
    if(argc != 3) {
        printf("forma de uso: %s <filename> <n>\n", argv[0]);
        exit(-1);
    }
    try {
        Cliente cliente(argv[1], (unsigned int) atoi(argv[2]));
        cliente.ejecutar();
    } catch(const char *msg) {
        perror(msg);
        // printf("%s\n", msg);
        exit(-1);
    }
    return 0;
}

