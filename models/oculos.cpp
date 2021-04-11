#include "oculos.h"

Oculos::Oculos()
{
    nome = "oculos";
    model = new Model3DS("../3ds/oculos.3DS");
}

void Oculos::desenhar()
{
    glPushMatrix();
        Objeto::desenhar();

        if(selecionado) {
            if (desenha_eixos)
                GUI::drawOrigin(0.5);
            GUI::setColor(1,1,1);
        }

        glScalef(1,1,1); //ajuste final da escala, podendo ser não-uniforme, independente para cada eixo
        glRotatef(-90,1,0,0); //alinhar o objeto 3ds com os eixos, deixando ele para cima de acordo com o eixo Y
        glTranslatef(0,0,0); //trazer objeto 3ds para origem
        glScalef(0.0025,0.0025,0.0025); //apenas para conseguir enxergar o modelo 3ds

        model->draw(!selecionado);
    glPopMatrix();
}
