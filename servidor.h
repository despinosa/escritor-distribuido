#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "archivo.h"
#include <vector>

using namespace std;

class Servidor {
public:
    Servidor();
    ~Servidor();
    void ejecutar(unsigned int repeticiones);
protected:
    vector<Archivo> archivos;
    int mi_descriptor, otro_descriptor;
    unsigned int n;
    char *ruta;
    int aceptar();
    void recibir(size_t nbytes, char *buffer);
    int recibir_n();
    void palabra_aleatoria(char *buffer, char *palabra);
    void cerrar();
};

#endif
