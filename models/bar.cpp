#include "bar.h"

Bar::Bar() { nome = "bar"; }

void Bar::bar()
{
    //face frente
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glVertex3f(0,0,1);
        glVertex3f(1,0,1);
        glVertex3f(1,1,1);
        glVertex3f(0,1,1);
    glEnd();
    //face tras
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,0,0);
    glEnd();
    //face direita
    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(1,0,0);
        glVertex3f(1,1,0);
        glVertex3f(1,1,1);
    glEnd();
    //face esquerda
    glBegin(GL_QUADS);
        glNormal3f(-1,0,0);
        glVertex3f(0,0,1);
        glVertex3f(0,1,1);
        glVertex3f(0,1,0);
        glVertex3f(0,0,0);
    glEnd();
    //face superior
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(0,1,0);
        glVertex3f(0,1,1);
        glVertex3f(1,1,1);
        glVertex3f(1,1,0);
    glEnd();
    //face inferior
    glBegin(GL_QUADS);
        glNormal3f(0,-1,0);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);
        glVertex3f(1,0,1);
        glVertex3f(0,0,1);
    glEnd();
    //telhado
    //esquerda
    glBegin(GL_POLYGON);
        glNormal3f(-1,1,0);
        glVertex3f(-0.2,1,-0.2);
        glVertex3f(-0.2,1,1.2);
        glVertex3f(0.5,1.2,0.5);
    glEnd();
    //frente
    glBegin(GL_POLYGON);
        glNormal3f(0,1,1);
        glVertex3f(-0.2,1,1.2);
        glVertex3f(1.2,1,1.2);
        glVertex3f(0.5,1.2,0.5);
    glEnd();
    //tras
    glBegin(GL_POLYGON);
        glNormal3f(0,1,-1);
        glVertex3f(1.2,1,-0.2);
        glVertex3f(-0.2,1,-0.2);
        glVertex3f(0.5,1.2,0.5);
    glEnd();
    //direita
    glBegin(GL_POLYGON);
        glNormal3f(1,1,0);
        glVertex3f(1.2,1,1.2);
        glVertex3f(1.2,1,-0.2);
        glVertex3f(0.5,1.2,0.5);
    glEnd();
}

void Bar::desenhar()
{
    glPushMatrix();
        Objeto::desenhar();

        if (selecionado) {
            if (desenha_eixos)
                GUI::drawOrigin(0.5);
            GUI::setColor(1,1,1);
        } else {
            GUI::setColor(0.96,0.83,0.15);
        }

        bar();

    glPopMatrix();
}
