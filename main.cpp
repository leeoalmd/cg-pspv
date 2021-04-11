#include <iostream>

using namespace std;

#include <gui.h>
#include <vector>

#include <iostream>
#include <vector>
#include <fstream>

#include "models/bar.h"
#include "models/oculos.h"
#include "models/sanfona.h"
#include "models/freezer.h"
#include "models/sinuca.h"

#include "models/objeto.h"

using namespace std;

vector<Objeto*> objetos_cena;
int pos_selecionado = -1;

//-------------------sombra-------------------
bool drawShadow = true;
bool pontual = true;
float k = 0.0;
//-------------------sombra-------------------

vector<Camera*> cameraOrtho;
vector<Camera*> cameraP;
vector<Camera*> cameraOb;
int marcaCameraOb = -1;
int marcaCamera = -1;
int marcaCameraO = -1;
int marcaCameraP = -1;

bool perspective = false;
bool obliq = false;
bool ortho = false;

void sombra(GLfloat plano[4], GLfloat lightPos[4]){

    glPushMatrix();

        GLfloat sombra[4][4];
        GUI::shadowMatrix(sombra,plano,lightPos);
        glMultTransposeMatrixf( (GLfloat*)sombra );

        glDisable(GL_LIGHTING);
        glColor3d(0.0,0.0,0.0);

        for (int i = 0; i < objetos_cena.size(); ++i) {

            if(objetos_cena[i]->sombra){
                glPushMatrix();
                    objetos_cena[i]->desenhar();
                glPopMatrix();
            }
        }
        glEnable(GL_LIGHTING);
    glPopMatrix();

}

void sombra_x(){
    //definindo a luz que sera usada para gerar a sombra
    float lightPos2[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano2[4] = {1,0,0,2.49}; //{Normal, x, y, z}
    sombra(plano2, lightPos2);
}

void sombra_y(){    //definindo a luz que sera usada para gerar a sombra
    float lightPos[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano[4] = {0,1,0,-0.001}; //{Normal, x, y, z}
    sombra(plano, lightPos);
}

void sombra_z(){
    //definindo a luz que sera usada para gerar a sombra
    float lightPos3[4] = {0+glutGUI::lx,3+glutGUI::ly,3+glutGUI::lz,pontual};
    //GUI::setLight(0,lightPos[0],lightPos[1],lightPos[2],true,false,false,false,pontual);
    GUI::setLight(0,0,3,3,true,false,false,false,pontual);
    //desenhando os objetos projetados
    GLfloat plano3[4] = {-1,0,0,2.49}; //{Normal, x, y, z}
    sombra(plano3, lightPos3);
}

//-------------------picking------------------
//vector<Vetor3D> pontosControle;

int pontoSelecionado = 0; //names = [1,n] //n = pontosControle.size()
//bool transPontos = glutGUI::trans_obj; //= true;

void desenhaPontosDeControle()
{
    for (int i = 0; i < objetos_cena.size(); ++i) {
        //definindo cor da selecao

        if (i == pontoSelecionado-1) {
            objetos_cena[i]->selecionado = true;
        } else {
            objetos_cena[i]->selecionado = false;
        }

        glPushMatrix();
        glPushName(i+1); //não se deve definir name = 0!
            objetos_cena[i]->desenhar();
        glPopName();
        glPopMatrix();
    }
}

//picking
int picking( GLint cursorX, GLint cursorY, int w, int h ) {
    int BUFSIZE = 512;
    GLuint selectBuf[512];

    GUI::pickingInit(cursorX,cursorY,w,h,selectBuf,BUFSIZE);

//de acordo com a implementacao original da funcao display
    //lembrar de nao inicializar a matriz de projecao, para nao ignorar a gluPickMatrix
    GUI::displayInit();
    //só precisa desenhar o que for selecionavel
    desenhaPontosDeControle();
//fim-de acordo com a implementacao original da funcao display

    //retornando o name do objeto (ponto de controle) mais proximo da camera (z minimo! *[matrizes de normalizacao da projecao])
    return GUI::pickingClosestName(selectBuf,BUFSIZE);
}
//-------------------picking------------------


//----------- manipulacao arquivo  ------------
void salvar_arquivo()
{
    ofstream arquivo("../objetos.txt");

    if (arquivo.is_open()) {
        if (objetos_cena.empty()) {
            cout << "Sem objetos em cena para salvar" << endl;
        } else {
            for (int i = 0; i < objetos_cena.size(); i++) {
                arquivo << objetos_cena[i]->nome << " ";

                arquivo << objetos_cena[i]->tr.x << " ";
                arquivo << objetos_cena[i]->tr.y << " ";
                arquivo << objetos_cena[i]->tr.z << " ";

                arquivo << objetos_cena[i]->rt.x << " ";
                arquivo << objetos_cena[i]->rt.y << " ";
                arquivo << objetos_cena[i]->tr.z << " ";

                arquivo << objetos_cena[i]->sc.x << " ";
                arquivo << objetos_cena[i]->sc.y << " ";
                arquivo << objetos_cena[i]->sc.z << endl;

                cout << "Objeto salvo" << endl;
            }
        }

        arquivo.close();
    } else {
        cout << "Erro de leitura" << endl;
    }
}

void carregar_arquivo()
{
    ifstream arquivo("../objetos.txt");

    if(!arquivo) {
        cout << "Erro de leitura" << endl;
    }

    string nome_objeto = "";
    float tr_x = 0, tr_y = 0, tr_z = 0;
    float rt_x = 0, rt_y = 0, rt_z = 0;
    float sc_x = 1, sc_y = 1, sc_z = 1;

    Vetor3D tr_ = Vetor3D(0,0,0);
    Vetor3D rt_ = Vetor3D(0,0,0);
    Vetor3D sc_ = Vetor3D(1,1,1);

    while(!arquivo.eof()) {
        arquivo >> nome_objeto;

        if (nome_objeto == "bar") {
            arquivo >> tr_x >> tr_y >> tr_z;
            arquivo >> rt_x >> rt_y >> rt_z;
            arquivo >> sc_x >> sc_y >> sc_z;

            Bar* bar = new Bar ();

            bar->tr.x = tr_x;
            bar->tr.y = tr_y;
            bar->tr.z = tr_z;

            bar->rt.x = rt_x;
            bar->rt.y = rt_y;
            bar->rt.z = rt_z;

            bar->sc.x = sc_x;
            bar->sc.y = sc_y;
            bar->sc.z = sc_z;

            objetos_cena.push_back(bar);
        } else if (nome_objeto == "freezer") {
            arquivo >> tr_.x >> tr_.y >> tr_.z;
            arquivo >> rt_.x >> rt_.y >> rt_.z;
            arquivo >> sc_.x >> sc_.y >> sc_.z;

            Freezer* freezer = new Freezer ();

            freezer->tr = tr_;
            freezer->rt = rt_;
            freezer->sc = sc_;

            objetos_cena.push_back(freezer);
        } else if (nome_objeto == "oculos") {
            arquivo >> tr_x >> tr_y >> tr_z;
            arquivo >> rt_x >> rt_y >> rt_z;
            arquivo >> sc_x >> sc_y >> sc_z;

            Oculos* oculos = new Oculos ();

            oculos->tr.x = tr_x;
            oculos->tr.y = tr_y;
            oculos->tr.z = tr_z;

            oculos->rt.x = rt_x;
            oculos->rt.y = rt_y;
            oculos->rt.z = rt_z;

            oculos->sc.x = sc_x;
            oculos->sc.y = sc_y;
            oculos->sc.z = sc_z;

            objetos_cena.push_back(oculos);
        } else if (nome_objeto == "sanfona") {
            arquivo >> tr_.x >> tr_.y >> tr_.z;
            arquivo >> rt_.x >> rt_.y >> rt_.z;
            arquivo >> sc_.x >> sc_.y >> sc_.z;

            Sanfona* sanfona = new Sanfona ();

            sanfona->tr = tr_;
            sanfona->rt = rt_;
            sanfona->sc = sc_;

            objetos_cena.push_back(sanfona);
        } else if (nome_objeto == "sinuca") {
            arquivo >> tr_.x >> tr_.y >> tr_.z;
            arquivo >> rt_.x >> rt_.y >> rt_.z;
            arquivo >> sc_.x >> sc_.y >> sc_.z;

            Sinuca* sinuca = new Sinuca ();

            sinuca->tr = tr_;
            sinuca->rt = rt_;
            sinuca->sc = sc_;

            objetos_cena.push_back(sinuca);
        }
    }
}

//-------------------viewPorts------------------
bool viewports = true;
bool scissored = true;

void cenario();

//visao de duas cameras (duas viewports), viewport auxiliar sobrepondo a principal
void viewPorts() {
    float width = glutGUI::width;
    float height = glutGUI::height;

    //viewport principal
    glViewport(0, 0, width, height);
        glLoadIdentity();
        gluLookAt(glutGUI::cam->e.x,glutGUI::cam->e.y,glutGUI::cam->e.z, glutGUI::cam->c.x,glutGUI::cam->c.y,glutGUI::cam->c.z, glutGUI::cam->u.x,glutGUI::cam->u.y,glutGUI::cam->u.z);
            cenario();

    //viewport auxiliar sobrepondo a principal
    if (scissored) {
        Vetor3D eye;
        Vetor3D center;
        //misturando com a principal
        int largura_vp = 200;
        int altura_vp = 100;
        int borda = 20;

        //inferior esquerdo
        GUI::glScissoredViewport(0, 0, largura_vp, altura_vp);
            //glLoadIdentity();
            eye = Vetor3D(4,4,4);
            center = Vetor3D(-1,0,-1);
            gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
            cenario();
        // superior esquerdo
        GUI::glScissoredViewport(0, altura_vp+borda, largura_vp, altura_vp);
            //glLoadIdentity();
            eye = Vetor3D(0,3,4);
            center = Vetor3D(-1,3,-1);
            gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
            cenario();
        // inferior direito
        GUI::glScissoredViewport(largura_vp+borda, 0, largura_vp, altura_vp);
            //glLoadIdentity();
            eye = Vetor3D(2,1,1);
            center = Vetor3D(3,3,1);
            gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
            cenario();
        //superior direito
        GUI::glScissoredViewport(largura_vp+borda, altura_vp+borda, largura_vp, altura_vp);
            //glLoadIdentity();
            eye = Vetor3D(3,3,3);
            center = Vetor3D(0,0,0);
            gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
            cenario();
    }
    else {
        //recortando/substituindo o pedaço
        GUI::glScissoredViewport(0, 3*height/4, width/4, height/4);
    }
//        glLoadIdentity();
//        Vetor3D eye = Vetor3D(4,4,4);
//        Vetor3D center = Vetor3D(-1,0,-1);
//        gluLookAt(eye.x,eye.y,eye.z, center.x,center.y,center.z, 0.0,1.0,0.0);
//            cenario();
}
//-------------------viewPorts------------------

void parede_x()
{
    glPushMatrix();
    GUI::setColor(0.3,0.3,0.3);
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(2.5,0,2.5);
        glVertex3f(2.5,3.5,2.5);
        glVertex3f(2.5,3.5,-2.5);
        glVertex3f(2.5,0,-2.5);
    glEnd();
    glPopMatrix();
}

void parede_z()
{
    glPushMatrix();
    GUI::setColor(0.3,0.3,0.3);
    glBegin(GL_QUADS);
        glNormal3f(1,0,0);
        glVertex3f(-2.5,0,2.5);
        glVertex3f(-2.5,0,-2.5);
        glVertex3f(-2.5,3.5,-2.5);
        glVertex3f(-2.5,3.5,2.5);
    glEnd();
    glPopMatrix();
}

void parede_s()
{
    glPushMatrix();
    GUI::setColor(0.3,0.3,0.3);
    glRotated(-15,1,0,0);
        glTranslated(0,0,-1.33+1.5);
        glBegin(GL_QUADS);
            glNormal3f(0,0,-1);
            glVertex3f(2.5,0,-2.5);
            glVertex3f(2.5,3.5,-2.5);
            glVertex3f(-2.5,3.5,-2.5);
            glVertex3f(-2.5,0,-2.5);
        glEnd();
    glPopMatrix();
}

void cenario() {
    //GUI::setLight(1,1,3,5,true,false);               //multiplas fontes de luz
    //GUI::setLight(2,-1.5,0.5,-1,true,false);         //multiplas fontes de luz
    GUI::setLight(3,-5,3,5,true,false);
    //GUI::setLight(3,-5,3,5,true,true); //atenuada
    //GUI::setLight(3,-5,3,5,true,false,false,false,true,true); //spot (holofote, lanterna)

    //GUI::setColor(1,0,0);
    //GUI::drawFloor();

    //GUI::setLight(1,1,3,5,true,false);
    //GUI::setLight(2,-1.5,0.5,-1,true,false);

    GUI::setColor(0.34,0.2,0.08);
    GUI::drawFloor();

    for (int i = 0; i < objetos_cena.size(); ++i) {
        glPushMatrix();
            objetos_cena[i]->desenhar();
        glPopMatrix();
    }

    if (pos_selecionado >= 0 and pos_selecionado < objetos_cena.size()) {
        objetos_cena[pos_selecionado]->tr.x += glutGUI::dtx;
        objetos_cena[pos_selecionado]->tr.y += glutGUI::dty;
        objetos_cena[pos_selecionado]->tr.z += glutGUI::dtz;

        objetos_cena[pos_selecionado]->rt.x += glutGUI::dax;
        objetos_cena[pos_selecionado]->rt.y += glutGUI::day;
        objetos_cena[pos_selecionado]->rt.z += glutGUI::daz;

        objetos_cena[pos_selecionado]->sc.x += glutGUI::dsx;
        objetos_cena[pos_selecionado]->sc.y += glutGUI::dsy;
        objetos_cena[pos_selecionado]->sc.z += glutGUI::dsz;
    }

    desenhaPontosDeControle();

    sombra_x();
    sombra_y();
    sombra_z();

    parede_x();
    parede_z();
    parede_s();

}

void desenha() {
    GUI::displayInit();

    if (!viewports) {
        cenario();
    } else {
        viewPorts();
    }

    //transladando ponto selecionado atraves do picking   

    GUI::displayEnd();
}

void teclado(unsigned char key, int x, int y) {
    GUI::keyInit(key,x,y);

    switch (key) {
    //Inserção
    case '/':
        objetos_cena.push_back(new Bar ());
        break;
    case ']':
        objetos_cena.push_back(new Freezer ());
        break;
    case '[':
        objetos_cena.push_back(new Oculos ());
        break;
    case '.':
        objetos_cena.push_back(new Sinuca ());
        break;
    case ',':
        objetos_cena.push_back(new Sanfona ());
        break;
    //Alternância
    case '+': /*Próximo*/
        if (pos_selecionado >= 0 and pos_selecionado < objetos_cena.size()) {
            objetos_cena[pos_selecionado]->selecionado = false;
            objetos_cena[pos_selecionado]->desenha_eixos = false;
        }
        pos_selecionado++;
        pos_selecionado = pos_selecionado % objetos_cena.size();
        if (pos_selecionado >= 0 and pos_selecionado < objetos_cena.size()) {
            objetos_cena[pos_selecionado]->selecionado = true;
            objetos_cena[pos_selecionado]->desenha_eixos = true;
        }
        break;
    case '-': /*Anterior*/
        if (pos_selecionado >= 0 and pos_selecionado < objetos_cena.size()) {
            objetos_cena[pos_selecionado]->selecionado = false;
            objetos_cena[pos_selecionado]->desenha_eixos = false;
        }
        pos_selecionado--;
        if (pos_selecionado < 0) {
            pos_selecionado = objetos_cena.size()-1;
        }
        if (pos_selecionado >= 0 and pos_selecionado < objetos_cena.size()) {
            objetos_cena[pos_selecionado]->selecionado = true;
            objetos_cena[pos_selecionado]->desenha_eixos = true;
        }
        break;
    //Exclusão
    case 8: /*Backspace*/
        if (!objetos_cena.empty()) {
            cout << pos_selecionado << " " << objetos_cena.at(pos_selecionado)->nome << " fora de cena" << endl;
            objetos_cena.erase(objetos_cena.begin() + (pos_selecionado));
        }
        break;
    //Arquivo
    case '>':
        salvar_arquivo();
        break;
    case '<':
        carregar_arquivo();
        break;
    //Viewports
    case 'v':
        viewports = !viewports;
        break;
    case 's':
        scissored = !scissored;
        break;
    //Projeções
    case '=':
        ortho = true;
        perspective= false;
        obliq = false;
        marcaCameraO++;

        if(marcaCameraO>=cameraOrtho.size()){
            marcaCameraO =0;
        }
        glutGUI::cam = new CameraDistante(cameraOrtho[marcaCameraO]->e, cameraOrtho[marcaCameraO]->c, cameraOrtho[marcaCameraO]->u);
        break;
    case '~':
        perspective = true;
        obliq = false;
        ortho = false;
        marcaCameraP++;

        if(marcaCameraP>=cameraP.size()){
            marcaCameraP =0;
        }
        glutGUI::cam = new CameraDistante(cameraP[marcaCameraP]->e, cameraP[marcaCameraP]->c, cameraP[marcaCameraP]->u);
        break;
    case ';':
        obliq = true;
        ortho = false;
        perspective = false;
        marcaCameraOb++;
        if(marcaCameraOb>=cameraOb.size()){
            marcaCameraOb = 0;
        }
        glutGUI::cam = new CameraDistante(cameraOb[marcaCameraOb]->e, cameraOb[marcaCameraOb]->c, cameraOb[marcaCameraOb]->u);
        break;
    //Padrões
    case 't':
        glutGUI::trans_obj = !glutGUI::trans_obj;
        break;
    case 'l':
        glutGUI::trans_luz = !glutGUI::trans_luz;
        break;
    default:
        break;
    }
}

void mouse(int button, int state, int x, int y) {
    GUI::mouseButtonInit(button,state,x,y);

    // if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        // when the button is pressed
        if (state == GLUT_DOWN) {
            //picking
            int pick = picking( x, y, 5, 5 );
            if (pick != 0) {
                pontoSelecionado = pick;
                glutGUI::lbpressed = false;
            }
        }
    }
}

int main()
{
    cout << "Hello World!" << endl;
    //carregar_arquivo();

    //isometrica
    cameraOrtho.push_back(new CameraDistante(0,1,5, 0,1,0, 0,1,0));
    //trimetrica
    cameraOrtho.push_back(new CameraDistante(5,1,0, 0,1,0, 0,1,0));
    //dimetrica
    cameraOrtho.push_back(new CameraDistante(0,1,-5, 0,1,0, 0,1,0));
    //1ponto
    cameraP.push_back(new CameraDistante(-5,1,0, 0,1,0, 0,1,0));
    //2ponto
    cameraP.push_back(new CameraDistante(0,6,0, 0,1,0, 0,0,-1));
    //3ponto
    cameraP.push_back(new CameraDistante(10,2,1, -2,1,0, 0,1,0));
    cameraOb.push_back(new CameraDistante(10,2,1, -2,1,0, 0,1,0));

    GUI gui = GUI(900,600,desenha,teclado,mouse);
}


//while(true) {
//    desenha();
//    interacaoUsuario();
//}
