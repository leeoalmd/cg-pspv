#include "objeto.h"

Objeto::Objeto() {}

void Objeto::desenhar()
{
    //Composição das transformações
    glTranslatef(tr.x,tr.y,tr.z);
    glRotatef(rt.z,0,0,1);
    glRotatef(rt.y,0,1,0);
    glRotatef(rt.x,1,0,0);
    glScalef(sc.x,sc.y,sc.z);
}
