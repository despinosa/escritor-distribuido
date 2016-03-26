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

void Servidor::inicializar() {
    descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(descriptor < 0) {
        throw "error al crear socket";
    }
}

Servidor::Servidor(const char *filename, unsigned int n) {
    archivo = Archivo(filename, O_RDONLY);
    this->n = n;
    inicializar();
}

Servidor::~Servidor() {
    archivo.cerrar();
    close(descriptor);
}

void Servidor::conectar() {
}

void Servidor::recibir(char *str) {
    if(send(descriptor, str, strlen(str), 0) < 0) {
        throw "error al recibir";
    }
}
void Servidor::recibir_n() {
    if(send(descriptor, &n, sizeof(int), 0) < 0) {
        throw "error al recibir n";
    }
}

void Servidor::ejecutar() {
    size_t leido;
    conectar();
    recibir_n();
    for(int i = 0; i < n; ++i) {
        leido = archivo.lee(TAM_DGRAMA);
        if(leido < TAM_DGRAMA) break;
        recibir(archivo.get_contenido());
    }
}

int main(int argc, char const *argv[]) {
    Servidor servidor;
    if(argc != 1) {
        printf("no recibo parámetros\n", argv[0]);
        exit(-1);
    }
    try {
        servidor = Servidor();
        servidor.ejecutar();
    } catch(char *msg) {
        perror(msg);
        printf("%s\n", msg);
        exit(-1);
    }
    return 0;
}

