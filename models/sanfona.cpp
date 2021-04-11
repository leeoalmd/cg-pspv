#include "sanfona.h"

Sanfona::Sanfona()
{
    nome = "sanfona";
    model = new Model3DS("../3ds/sanfona.3DS");
}

void Sanfona::desenhar()
{
    glPushMatrix();
        Objeto::desenhar();

        if (selecionado) {
            if (desenha_eixos)
                GUI::drawOrigin(0.5);
            GUI::setColor(1,1,1);
        }

        glScalef(1,1,1); //ajuste final da escala, podendo ser não-uniforme, independente para cada eixo
        glRotatef(-90,1,0,0); //alinhar o objeto 3ds com os eixos, deixando ele para cima de acordo com o eixo Y
        glTranslatef(0,0,0); //trazer objeto 3ds para origem
        glScalef(0.001,0.001,0.001); //apenas para conseguir enxergar o modelo 3ds

        model->draw(!selecionado);
    glPopMatrix();
}
