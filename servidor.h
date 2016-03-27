#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "archivo.h"
#include <vector>
#include <sys/un.h>

using namespace std;

class Servidor {
public:
    Servidor();
    ~Servidor();
    void ejecutar(unsigned int repeticiones);
protected:
    vector<Archivo> archivos;
    struct sockaddr_un local, remota;
    int descriptor;
    unsigned int n;
    char *ruta;
    char *recibir(size_t nbytes);
    int recibir_n();
    char *palabra_aleatoria(char *buffer);
    void cerrar();
};

#endif
