#ifndef OBJETO_H
#define OBJETO_H

#include <string>

#include <Desenha.h>
#include <gui.h>

class Objeto
{
public:
    Vetor3D tr = Vetor3D(0,0,0);
    Vetor3D rt = Vetor3D(0,0,0);
    Vetor3D sc = Vetor3D(1,1,1);

    bool selecionado = false, desenha_eixos = false, sombra = true;

    std::string nome = "";

    Objeto();
    virtual void desenhar();
};

#endif // OBJETO_H
