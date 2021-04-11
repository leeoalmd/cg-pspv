#ifndef FREEZER_H
#define FREEZER_H

#include "models/objeto.h"

class Freezer : public Objeto
{
public:
    Freezer();
    void freezer();

    void desenhar();
};

#endif // FREEZER_H
