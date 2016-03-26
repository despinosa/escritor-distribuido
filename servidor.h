#ifndef CLIENTE_H
#define CLIENTE_H

#include "archivo.h"

class Servidor {
public:
    Servidor();
    ~Servidor();
    void ejecutar();
protected:
    Archivo archivo;
    int descriptor;
    unsigned int n;
    char *ruta;
    void inicializar();
    void conectar();
    void recibir(char *str);
    void recibir_n();
    void cerrar();
};

#endif
