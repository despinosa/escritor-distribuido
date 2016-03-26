#include "cliente.h"
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

Cliente::Cliente(const char *filename, unsigned int n) {
    archivo = Archivo(filename, O_RDONLY);
    this->n = n;
    inicializar();
}

Cliente::~Cliente() {
    archivo.cerrar();
    close(descriptor);
}

void Cliente::ejecutar() {
    size_t leido;
    conectar();
    enviar_n();
    for (int i = 0; i < n; ++i) {
        leido = archivo.lee(TAM_DGRAMA);
        if (leido < TAM_DGRAMA) break;
        enviar(archivo.get_contenido());
    }
}

void Cliente::inicializar() {
    descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(descriptor < 0) {
        throw "error al crear socket";
    }
}

void Cliente::conectar() {
    struct sockaddr_un remoto;
    remoto.sun_family = AF_UNIX;
    strcpy(remoto.sun_path, SOCK_PATH);
    len = strlen(remoto.sun_path) + sizeof(remoto.sun_family);
    if(connect(descriptor, (struct sockaddr*) &remoto, len) < 0) {
        throw "error al conectar";
    }
}

void Cliente::enviar(char *str) {
    if(send(descriptor, str, strlen(str), 0) > 0) {
        throw "error al enviar";
    }
}
void Cliente::enviar_n() {
    send(descriptor, &n, sizeof(int), 0)
}

int main(int argc, char const *argv[]) {
    Cliente cliente;
    if(argc != 3) {
        printf("forma de uso: %s <filename> <n>\n", argv[0]);
        exit(-1);
    }
    cliente = Cliente(argv[1], (unsigned int) atoi(argv[2]));
    cliente.ejecutar();
    return 0;
}

