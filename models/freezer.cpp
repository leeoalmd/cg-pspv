#include "freezer.h"

Freezer::Freezer() { nome = "freezer"; }

void Freezer::freezer()
{
    //face frente
    glBegin(GL_QUADS);
        glNormal3f(0,0,1);
        glVertex3f(0,0,0.25);
        glVertex3f(0.25,0,0.25);
        glVertex3f(0.25,0.75,0.25);
        glVertex3f(0,0.75,0.25);
    glEnd();
    //face tras
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(0,0,0);
        glVertex3f(0,0.75,0);
        glVertex3f(0.25,0.75,0);
        glVertex3f(0.25,0,0);
    glEnd();
    //face direita
    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glVertex3f(0.25,0,0.25);
        glVertex3f(0.25,0,0);
        glVertex3f(0.25,0.75,0);
        glVertex3f(0.25,0.75,0.25);
    glEnd();
    //face esquerda
    glBegin(GL_QUADS);
        glNormal3f(-1,0,0);
        glVertex3f(0,0,0.25);
        glVertex3f(0,0.75,0.25);
        glVertex3f(0,0.75,0);
        glVertex3f(0,0,0);
    glEnd();
    //face superior
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(0,0.75,0);
        glVertex3f(0,0.75,0.25);
        glVertex3f(0.25,0.75,0.25);
        glVertex3f(0.25,0.75,0);
    glEnd();
    //face inferior
    glBegin(GL_QUADS);
        glNormal3f(0,-1,0);
        glVertex3f(0,0,0);
        glVertex3f(0.25,0,0);
        glVertex3f(0.25,0,0.25);
        glVertex3f(0,0,0.25);
    glEnd();
}

void Freezer::desenhar()
{
    glPushMatrix();
        Objeto::desenhar();

        if (selecionado) {
            if (desenha_eixos)
                GUI::drawOrigin(0.5);
            GUI::setColor(1,1,1);
        } else {
            GUI::setColor(0.75,0.75,0.75);
        }

        freezer();
    glPopMatrix();
}
