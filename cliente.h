#ifndef CLIENTE_H
#define CLIENTE_H

#include "archivo.h"
#include <sys/un.h>

class Cliente {
public:
    Cliente(const char *filename, unsigned int n);
    ~Cliente();
    void ejecutar();
protected:
    Archivo archivo;
    struct sockaddr_un local, remota;
    int descriptor;
    unsigned int n;
    char *ruta;
    void conectar();
    void enviar(char *str);
    void enviar_n();
    void cerrar();
};

#endif
