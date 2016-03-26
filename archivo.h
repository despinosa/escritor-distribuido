#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <string>

using namespace std;

class Archivo {
public:
    Archivo(const char *filename, int banderas);
    Archivo(const char *filename, int banderas, mode_t modo);
    ~Archivo();
    size_t lee(size_t nbytes);
    size_t escribe(const void *buffer, size_t nbytes);
    char *get_contenido();
    void cerrar();
protected:
    string nombre_archivo;
    int descriptor;
    char *contenido;
};

#endif
