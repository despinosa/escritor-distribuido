#ifndef CLIENTE_H
#define CLIENTE_H

#include "archivo.h"

using namespace std;

class Servidor {
public:
    Servidor();
    ~Servidor();
    void ejecutar();
protected:
    vector<Archivo> archivos;
    int mi_descriptor, otro_descriptor;
    unsigned int n;
    char *ruta;
    void inicializar();
    void aceptar();
    string recibir(char *str);
    void recibir_n();
    void cerrar();
};

#endif
