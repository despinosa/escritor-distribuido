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
    char *ruta, *buffer;
    void aceptar();
    char *recibir();
    void recibir_n();
    char *palabra_aleatoria();
    void cerrar();
};

#endif
