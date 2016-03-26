#include "archivo.h"
#include <fcntl.h>
#include <stdlib.h>

Archivo::Archivo(const char *filename, int banderas) {
    descriptor = open(filename, banderas);
    if(descriptor < 0) throw "error al abrir archivo";
    nombre_archivo = string(filename);
    contenido = (char*) malloc(1);
}

Archivo::Archivo(const char *filename, int banderas, mode_t modo) {
    descriptor = open(filename, banderas, modo);
    if(descriptor < 0) throw "error al abrir archivo";
    nombre_archivo = string(filename);
    contenido = (char*) malloc(1);
}

Archivo::~Archivo() {}

size_t Archivo::lee(size_t nbytes) {
    contenido = (char*) realloc(contenido, nbytes);
    return read(descriptor, contenido, nbytes);
}

size_t Archivo::escribe(const void *buffer, size_t nbytes) {
    return write(descriptor, buffer, nbytes);
}

char *Archivo::get_contenido() {
    return contenido;
}

void Archivo::cerrar() {
    if(close(descriptor) < 0) throw "error al cerrar archivo";
}

