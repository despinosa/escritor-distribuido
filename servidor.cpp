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
    struct sockaddr_un local;
    mi_descriptor = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(mi_descriptor < 0) {
        throw "error al crear socket";
    }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) < 0) {
        throw "error al registrar";
    }
    if (listen(s, 5) < 0) {
        throw "error al escuchar"
    }
}

Servidor::Servidor() {
    inicializar();
}

Servidor::~Servidor() {
    archivo.cerrar();
    close(mi_descriptor);
}

void Servidor::aceptar() {
    struct sockaddr_un remoto;
    int len;
    len = sizeof(remoto);
    otro_descriptor = accept(mi_descriptor, (struct sockaddr*) &remoto, &len);
    if (otro_descriptor < 0) {
        throw "error al aceptar"
    }
}

char* Servidor::recibir() {
    char buffer[TAM_DGRAMA];
    if(recv(otro_descriptor, buffer, strlen(buffer), 0) < 0) {
        throw "error al recibir";
    }
    return buffer;
}

void Servidor::recibir_n() {
    if(recv(otro_descriptor, &n, sizeof(int), 0) < 0) {
        throw "error al recibir n";
    }
}

void Servidor::ejecutar() {
    Archivo archivo;
    char filename[42], buffer[TAM_DGRAMA];
    while(1) {
        aceptar();
        recibir_n();
        for(int i = 0; i < n; ++i) {
            sprintf(filename, "%d.txt", i);
            archivo = Archivo(filename, O_RDONLY);
            buffer = recibir();
            archivo.escribe(buffer, strlen(buffer));
            archivos.push_back(archivo);
        }
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

