#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include "materiais.h"
#include <stdio.h>
#include <math.h>
#define BLACK    0.0, 0.0, 0.0, 1.0
#define frand()			((float)rand()/RAND_MAX)
#define MAX_PARTICULAS  2500

//================================================================================
//===========================================================Variaveis e constantes
GLfloat tamaltura=40;//Altura
GLfloat tam=60;//Lados
//----------------------------------------------------------------------------Controlos
GLint   roomtex=1;//textura da sala
GLint pontual=1;//Luz pontual controlo
GLint foco=1;//Foco controlo
GLint salto=0;//Controlo salto

//===============================================================================Animacoes?
GLfloat ang = 0.0f;//angulo incremento funclooptry
int aux;//Auxiliar funcjump
GLfloat saltoaltura= 0.0f;//salto incremento funcjump

//------------------------------------------------------------ Texturas def
GLuint  texture[64];
RgbImage imag;

//------------------------------------------------------------ Sistema Coordenadas
GLint		wScreen=800, hScreen=700;		//.. janela (pixeis)
GLfloat		xC=1.0, yC=1.0, zC=1.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Observador
GLfloat  PI = 3.14159;
GLfloat  rVisao=3.0, aVisao=0.5*PI, incVisao=0.1;
GLfloat  obsP[] ={30, 20, 30 ,1.0};
GLfloat  obsT[] ={obsP[0]-rVisao*cos(aVisao), obsP[1], obsP[2]-rVisao*sin(aVisao)};

//-------------------------------------------------------------Checkers stuff
struct coord{
	int x;
	int z;
};

GLint gamecamera =0;
int board[8][8] = {};
coord Checkerspos[8][8];
bool selected = false;
int sRow = 0;
int sCol = 0;
int turn = -1;

//--------------------------------------------------------------Fonte
//---------------------------------------- Particle attributes
typedef struct {
	float   size;		// tamanho
	float	life;		// vida
	float	fade;		// fade
	float	r, g, b;    // color
	GLfloat x, y, z;    // posicao
	GLfloat vx, vy, vz; // velocidade
    GLfloat ax, ay, az; // aceleracao
    bool active;
} Particle;

Particle  particula1[MAX_PARTICULAS];
Particle  particula2[MAX_PARTICULAS];
Particle  particula3[MAX_PARTICULAS];
GLint    milisec = 1000;


//===========================================================================Texturas
void criaDefineTexturas()
{

	//----------------------------------------- floor
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("tiles.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- Wood Room
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("wood.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//----------------------------------------  door
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("porta.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//----------------------------------------- ceiling
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("ceiling.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//----------------------------------------- brickroom
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("parede.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Roomborder
	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("wood2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Showcase
	glGenTextures(1, &texture[6]);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("azulejo.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Metal part of can
	glGenTextures(1, &texture[7]);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("metal.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Windows1
	glGenTextures(1, &texture[11]);
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("imag1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Windows2
	glGenTextures(1, &texture[12]);
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("imag2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Windows3
	glGenTextures(1, &texture[13]);
	glBindTexture(GL_TEXTURE_2D, texture[13]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("imag3.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Windows4
	glGenTextures(1, &texture[14]);
	glBindTexture(GL_TEXTURE_2D, texture[14]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("imag4.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Windows5
	glGenTextures(1, &texture[15]);
	glBindTexture(GL_TEXTURE_2D, texture[15]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("imag5.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Windows6
	glGenTextures(1, &texture[16]);
	glBindTexture(GL_TEXTURE_2D, texture[16]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("imag6.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- coca
	glGenTextures(1, &texture[17]);
	glBindTexture(GL_TEXTURE_2D, texture[17]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("COCA.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	glGenTextures(1, &texture[19]);
	glBindTexture(GL_TEXTURE_2D, texture[19]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("COCAMETAL.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	//--------------------------------------- Pepsi
	glGenTextures(1, &texture[18]);
	glBindTexture(GL_TEXTURE_2D, texture[18]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("PEPSI.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	glGenTextures(1, &texture[20]);
	glBindTexture(GL_TEXTURE_2D, texture[20]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	imag.LoadBmpFile("PEPSIMETAL.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
    //-------------------------------------------baseFonte
    glGenTextures(1, &texture[21]);
	glBindTexture(GL_TEXTURE_2D, texture[21]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("marmore.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
    //--------------------------------------------agua
    //-------------------------------------------baseFonte
    glGenTextures(1, &texture[22]);
	glBindTexture(GL_TEXTURE_2D, texture[22]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	imag.LoadBmpFile("water2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}

//================================================================================
//=========================================================================== INIT
//================================================================================
void initLights(){
	GLfloat ambient[]		= {0.0f, 0.0f,  0.0f, 1.0f};
	glEnable(GL_LIGHTING);
	//=================================================================Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);
	//================================================================Luz do tecto? Pontual
	glEnable(GL_LIGHT0);
	GLfloat qaAmbientLight[] = {0.4, 0.4, 0.4, 1.0f};
	GLfloat qaDiffuseLight[] = {1.0, 1.0, 1.0, 1.0f};
	GLfloat qaSpecularLight[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat localAttCon =1.0;
	GLfloat localAttLin =0.05;
	GLfloat localAttQua =0.0;
	glLightfv(GL_LIGHT0,GL_AMBIENT,				qaAmbientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,				qaDiffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,			qaSpecularLight);
	//Atenuacao faz a sala ficar demasiado escura
	//glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
	//glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, localAttLin);
	//glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION,localAttQua);
	GLfloat qaPosition[] = {30,39,30,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,				qaPosition);
	//-------------------------------------------------------------------Foco
	GLfloat spot_direction[] = { 0,0, -1,1 };
	GLfloat pos [] = {0,0,0,1};
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1,GL_AMBIENT,				qaAmbientLight);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,				qaDiffuseLight);
	glLightfv(GL_LIGHT1,GL_SPECULAR,			qaSpecularLight);
	glLightfv(GL_LIGHT1,GL_POSITION,				pos);
	glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,				90);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 45.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);

}


void InitBoard()//Inicializa a matrix de jogo
{	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if ((j % 2) == (i % 2)){
				if (i < 3){
					printf("%i %i 1\n", i, j);
					board[i][j] = 1;
				}
				else if (i > 4){
					printf("%i %i -1\n", i, j);
					board[i][j] = -1;
				}
				else {
					board[i][j] = 0;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------FONTE
//==========================================================

void iniParticulas(Particle *particula, int sistema)
{

    float ps = 0.1;
     for(int i=0;i<=MAX_PARTICULAS;i++){
        particula[i].size = ps ;		// tamanh de cada particula

        if(sistema==1){
            particula[i].x = 30.0f;
            particula[i].y = 8.5f;
            particula[i].z = 55.0f;
        }else if(sistema==2){
            particula[i].x = 50.0f;
            particula[i].y = 8.5f;
            particula[i].z = 55.0f;
        }else{
            particula[i].x = 10.0f;
            particula[i].y = 8.5f;
            particula[i].z = 55.0f;
        }

        particula[i].vx = float((rand()%60)-32.0f);
        particula[i].vy = float((rand()%60)-30.0f);
        particula[i].vz = float((rand()%60)-30.0f);
        particula[i].ax = 0.0f;
        particula[i].ay = 5.0f;
        particula[i].az = 0.0f;

        particula[i].r = 0.7f;
        particula[i].g = 0.7f;
        particula[i].b = 1.0f;
        particula[i].life = 1.0f;
        particula[i].fade = float(rand()%100)/1000.0f+0.003f;
        particula[i].active = true;
     }
}

void init(void){

	criaDefineTexturas();
	glClearColor(BLACK);
	glShadeModel(GL_SMOOTH);
	InitBoard();
	initLights();
	glEnable(GL_DEPTH_TEST);
	iniParticulas(particula1, 1);
	iniParticulas(particula2, 2);
	iniParticulas(particula3, 3);
}


//===========================================================================CENAS DA SALA EM SI
void door(){
	//Porta
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glPushMatrix();
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(26,30,0.1);
		glTexCoord2f(0, 0); glVertex3f(26,0,0.1);
		glTexCoord2f(1, 0); glVertex3f(34,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(34,30,0.1);

		glTexCoord2f(0, 1); glVertex3f(26,30,0.1);
		glTexCoord2f(0, 0); glVertex3f(26,30,0);
		glTexCoord2f(1, 0); glVertex3f(26,0,0);
		glTexCoord2f(1, 1); glVertex3f(26,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(34,30,0.1);
		glTexCoord2f(0, 0); glVertex3f(34,30,0);
		glTexCoord2f(1, 0); glVertex3f(34,0,0);
		glTexCoord2f(1, 1); glVertex3f(34,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(26,30,0);
		glTexCoord2f(0, 0); glVertex3f(34,30,0);
		glTexCoord2f(1, 0); glVertex3f(34,30,0.1);
		glTexCoord2f(1, 1); glVertex3f(26,30,0.1);


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}

void window(GLfloat transx, GLfloat transy, GLfloat transz,int angle,int rotatex,int rotatey,int rotatez,int x){
	//Janela
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	if(x!=99)
		glBindTexture(GL_TEXTURE_2D,texture[x]);
	else{
		if(turn==-1)
			glBindTexture(GL_TEXTURE_2D,texture[18]);
		else
			glBindTexture(GL_TEXTURE_2D,texture[17]);
	}
	glPushMatrix();
		glTranslatef(transx,transy,transz);
		glRotatef(angle,rotatex,rotatey,rotatez);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(7,30,0.1);
		glTexCoord2f(0, 0); glVertex3f(7,20,0.1);
		glTexCoord2f(1, 0); glVertex3f(23,20,0.1);
		glTexCoord2f(1, 1); glVertex3f(23,30,0.1);

		glTexCoord2f(0, 1); glVertex3f(7,30,0.1);
		glTexCoord2f(0, 0); glVertex3f(7,30,0);
		glTexCoord2f(1, 0); glVertex3f(7,20,0);
		glTexCoord2f(1, 1); glVertex3f(7,20,0.1);

		glTexCoord2f(0, 1); glVertex3f(23,30,0.1);
		glTexCoord2f(0, 0); glVertex3f(23,30,0);
		glTexCoord2f(1, 0); glVertex3f(23,20,0);
		glTexCoord2f(1, 1); glVertex3f(23,20,0.1);

		glTexCoord2f(0, 1); glVertex3f(7,20,0);
		glTexCoord2f(0, 0); glVertex3f(23,20,0);
		glTexCoord2f(1, 0); glVertex3f(23,20,0.1);
		glTexCoord2f(1, 1); glVertex3f(7,20,0.1);

		glTexCoord2f(0, 1); glVertex3f(7,30,0);
		glTexCoord2f(0, 0); glVertex3f(23,30,0);
		glTexCoord2f(1, 0); glVertex3f(23,30,0.1);
		glTexCoord2f(1, 1); glVertex3f(7,30,0.1);


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}

void RoomBorder(GLfloat transx, GLfloat transy, GLfloat transz,int angle,int rotatex,int rotatey,int rotatez){
	//Bordas da sala
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[5]);
	glPushMatrix();
		glTranslatef(transx,transy,transz);
		glRotatef(angle,rotatex,rotatey,rotatez);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(0,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(0,0,0.1);
		glTexCoord2f(1, 0); glVertex3f(60,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(60,15,0.1);

		glTexCoord2f(0, 1); glVertex3f(0,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(0,15,0);
		glTexCoord2f(1, 0); glVertex3f(0,0,0);
		glTexCoord2f(1, 1); glVertex3f(0,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(60,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(60,15,0);
		glTexCoord2f(1, 0); glVertex3f(60,0,0);
		glTexCoord2f(1, 1); glVertex3f(60,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(0,0,0);
		glTexCoord2f(0, 0); glVertex3f(60,0,0);
		glTexCoord2f(1, 0); glVertex3f(60,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(0,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(0,15,0);
		glTexCoord2f(0, 0); glVertex3f(60,15,0);
		glTexCoord2f(1, 0); glVertex3f(60,15,0.1);
		glTexCoord2f(1, 1); glVertex3f(0,15,0.1);


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}

void RoomBorderback(){
	//Bordas da sala
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[5]);
	glPushMatrix();
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(0,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(0,0,0.1);
		glTexCoord2f(1, 0); glVertex3f(26,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(26,15,0.1);

		glTexCoord2f(0, 1); glVertex3f(0,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(0,15,0);
		glTexCoord2f(1, 0); glVertex3f(0,0,0);
		glTexCoord2f(1, 1); glVertex3f(0,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(26,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(26,15,0);
		glTexCoord2f(1, 0); glVertex3f(26,0,0);
		glTexCoord2f(1, 1); glVertex3f(26,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(0,0,0);
		glTexCoord2f(0, 0); glVertex3f(26,0,0);
		glTexCoord2f(1, 0); glVertex3f(26,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(0,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(0,15,0);
		glTexCoord2f(0, 0); glVertex3f(26,15,0);
		glTexCoord2f(1, 0); glVertex3f(26,15,0.1);
		glTexCoord2f(1, 1); glVertex3f(0,15,0.1);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(34,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(34,0,0.1);
		glTexCoord2f(1, 0); glVertex3f(60,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(60,15,0.1);

		glTexCoord2f(0, 1); glVertex3f(34,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(34,15,0);
		glTexCoord2f(1, 0); glVertex3f(34,0,0);
		glTexCoord2f(1, 1); glVertex3f(34,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(60,15,0.1);
		glTexCoord2f(0, 0); glVertex3f(60,15,0);
		glTexCoord2f(1, 0); glVertex3f(60,0,0);
		glTexCoord2f(1, 1); glVertex3f(60,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(34,0,0);
		glTexCoord2f(0, 0); glVertex3f(60,0,0);
		glTexCoord2f(1, 0); glVertex3f(60,0,0.1);
		glTexCoord2f(1, 1); glVertex3f(34,0,0.1);

		glTexCoord2f(0, 1); glVertex3f(34,15,0);
		glTexCoord2f(0, 0); glVertex3f(60,15,0);
		glTexCoord2f(1, 0); glVertex3f(60,15,0.1);
		glTexCoord2f(1, 1); glVertex3f(34,15,0.1);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}

void floor(){
	//--------------------------------------------------------Chao
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( 0,  0, 0 );
			glTexCoord2f(10.0f,0.0f); glVertex3i( tam, 0, 0 );
			glTexCoord2f(10.0f,10.0f); glVertex3i( tam, 0, tam);
			glTexCoord2f(0.0f,10.0f); glVertex3i( 0,  0,  tam);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);
}



void room(){//SALA
	floor();
	glEnable(GL_COLOR_MATERIAL);
	//--------------------------------------------------------Cima
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[4]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( 0,  tamaltura, tam );
			glTexCoord2f(10.0f,0.0f); glVertex3i( 0, tamaltura, 0 );
			glTexCoord2f(10.0f,10.0f); glVertex3i(tam, tamaltura, 0);
			glTexCoord2f(0.0f,10.0f); glVertex3i( tam,  tamaltura,  tam);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glNormal3f(0,1,0);
	//-------------------------------------------------------Controlo textura da sala
	glEnable(GL_TEXTURE_2D);
	if(roomtex)
		glBindTexture(GL_TEXTURE_2D,texture[3]);
	else
		glBindTexture(GL_TEXTURE_2D,texture[1]);
	//--------------------------------------------------------Esquerda
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( 0,  0, tam );
			glTexCoord2f(10.0f,0.0f); glVertex3i( 0, tamaltura, tam );
			glTexCoord2f(10.0f,10.0f); glVertex3i(0, tamaltura, 0);
			glTexCoord2f(0.0f,10.0f); glVertex3i( 0,  0,  0);
		glEnd();
	glPopMatrix();
	glNormal3f(0,1,0);
	//--------------------------------------------------------Tras
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( tam,  0, 0 );
			glTexCoord2f(10.0f,0.0f); glVertex3i( tam, tamaltura, 0 );
			glTexCoord2f(10.0f,10.0f); glVertex3i(0, tamaltura, 0);
			glTexCoord2f(0.0f,10.0f); glVertex3i( 0,  0,  0);
		glEnd();
	glPopMatrix();
	glNormal3f(0,1,0);
	//--------------------------------------------------------Direita
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( tam,  0, tam );
			glTexCoord2f(10.0f,0.0f); glVertex3i( tam, tamaltura, tam );
			glTexCoord2f(10.0f,10.0f); glVertex3i(tam, tamaltura, 0);
			glTexCoord2f(0.0f,10.0f); glVertex3i( tam,  0,  0);
		glEnd();
	glPopMatrix();
	glNormal3f(0,1,0);
	//--------------------------------------------------------Frente
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3i( tam,  tamaltura, tam );
			glTexCoord2f(10.0f,0.0f); glVertex3i( tam, 0, tam );
			glTexCoord2f(10.0f,10.0f); glVertex3i(0, 0, tam);
			glTexCoord2f(0.0f,10.0f); glVertex3i( 0,  tamaltura,  tam);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);
	door();
	//janelas e bordas tras
	RoomBorderback();
	window(0,0,0,0,0,0,0,11);
	window(30,0,0,0,0,0,0,12);
	//janelas esquerda e bordas
	RoomBorder(0,0,60,90,0,1,0);
	window(0,0,30,90,0,1,0,13);
	window(0,0,60,90,0,1,0,14);
	//janelas direita e bordas
	RoomBorder(60,0,0,-90,0,1,0);
	window(60,0,0,-90,0,1,0,15);
	window(60,0,30,-90,0,1,0,16);
	//bordas frente e turn advisor
	RoomBorder(60,0,60,180,0,1,0);
	window(60,0,60,180,0,1,0,99);
	window(30,0,60,180,0,1,0,99);

}

//------------------------------------------------------------------------------------------------------------------------objetos

void Lata(){
	glMaterialfv(GL_FRONT, GL_AMBIENT, jadeAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, jadeDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, jadeSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, jadeCoef);
	glPushMatrix();//BASE circulo
		gluDisk(gluNewQuadric(),0,0.4,32,32);
	glPopMatrix();
	glPushMatrix();//BASE Lata
		gluCylinder(gluNewQuadric(),0.4,0.5,0.2,32,32);
	glPopMatrix();
	glPushMatrix();//CORPO
		glTranslatef(0,0,0.2);
		gluCylinder(gluNewQuadric(),0.5,0.5,1.5,32,32);
	glPopMatrix();
	glPushMatrix();//TOPO lata
		glTranslatef(0,0,1.7);
		gluCylinder(gluNewQuadric(),0.5,0.4,0.2,32,32);
	glPopMatrix();
	glPushMatrix();//TOPO circulo
		glTranslatef(0,0,1.9);
		gluDisk(gluNewQuadric(),0,0.4,32,32);
	glPopMatrix();
	glNormal3f(0,1,0);

}


void ShowcaselowerTOP(GLfloat transx, GLfloat transy, GLfloat transz){
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[6]);
	glPushMatrix();
		glTranslatef(transx,transy,transz);
	glBegin(GL_QUADS);

		glTexCoord2f(0, 1); glVertex3f(0,10,0);
		glTexCoord2f(0, 0); glVertex3f(5,10,0);
		glTexCoord2f(1, 0); glVertex3f(5,10,5);
		glTexCoord2f(1, 1); glVertex3f(0,10,5);

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);
}

void Showcaselower(GLfloat transx, GLfloat transy, GLfloat transz){
	//showcase
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[6]);
	glPushMatrix();
		glTranslatef(transx,transy,transz);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(0,10,0);
		glTexCoord2f(0, 0); glVertex3f(0,0,0);
		glTexCoord2f(1, 0); glVertex3f(5,0,0);
		glTexCoord2f(1, 1); glVertex3f(5,10,0);

		glTexCoord2f(0, 1); glVertex3f(0,10,5);
		glTexCoord2f(0, 0); glVertex3f(0,0,5);
		glTexCoord2f(1, 0); glVertex3f(5,0,5);
		glTexCoord2f(1, 1); glVertex3f(5,10,5);

		glTexCoord2f(0, 1); glVertex3f(0,10,5);
		glTexCoord2f(0, 0); glVertex3f(0,10,0);
		glTexCoord2f(1, 0); glVertex3f(0,0,0);
		glTexCoord2f(1, 1); glVertex3f(0,0,5);

		glTexCoord2f(0, 1); glVertex3f(5,10,5);
		glTexCoord2f(0, 0); glVertex3f(5,10,0);
		glTexCoord2f(1, 0); glVertex3f(5,0,0);
		glTexCoord2f(1, 1); glVertex3f(5,0,5);


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}

void ShowcaseUpper(GLfloat transx, GLfloat transy, GLfloat transz){
	//ShowcaseUpper vidro
	glEnable(GL_COLOR_MATERIAL);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(transx,transy+10,transz);
	glColor4f(1.0,1.0,1.0,0.3);
	 glBegin(GL_QUADS);
		glVertex3f(0,5,0);
		glVertex3f(0,0,0);
		glVertex3f(5,0,0);
		glVertex3f(5,5,0);

		glVertex3f(0,5,5);
		glVertex3f(0,0,5);
		glVertex3f(5,0,5);
		glVertex3f(5,5,5);

		glVertex3f(0,5,5);
		glVertex3f(0,5,0);
		glVertex3f(0,0,0);
		glVertex3f(0,0,5);

		glVertex3f(5,5,5);
		glVertex3f(5,5,0);
		glVertex3f(5,0,0);
		glVertex3f(5,0,5);

		glVertex3f(0,5,0);
		glVertex3f(5,5,0);
		glVertex3f(5,5,5);
		glVertex3f(0,5,5);
	glEnd();

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);
}

void baseFonte(GLfloat transx, GLfloat transy, GLfloat transz, GLfloat comprimento, GLfloat largura, GLfloat altura, int tex, GLfloat rep){
	//showcase
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[tex]);
	glPushMatrix();
    glTranslatef(transx,transy,transz);
	glBegin(GL_QUADS);
        //tras
		glTexCoord2f(0, rep); glVertex3f(transx,transy+altura,transz);
		glTexCoord2f(0, 0); glVertex3f(transx,transy,transz);
		glTexCoord2f(rep, 0); glVertex3f(transx+comprimento,transy,transz);
		glTexCoord2f(rep, rep); glVertex3f(transx+comprimento,transy+altura,transz);

        //frente
		glTexCoord2f(0, rep); glVertex3f(transx,transy+altura,transz+largura);
		glTexCoord2f(0, 0); glVertex3f(transx,transy,transz+largura);
		glTexCoord2f(rep, 0); glVertex3f(transx+comprimento,transy,transz+largura);
		glTexCoord2f(rep, rep); glVertex3f(transx+comprimento,transy+altura,transz+largura);

        //lado esquerdo
		glTexCoord2f(0, rep); glVertex3f(transx,transy+altura,transz+largura);
		glTexCoord2f(0, 0); glVertex3f(transx,transy+altura,transz);
		glTexCoord2f(rep, 0); glVertex3f(transx,transy,transz);
		glTexCoord2f(rep, rep); glVertex3f(transx,transy,transz+largura);

        //lado direito
		glTexCoord2f(0, rep); glVertex3f(transx+comprimento,transy+altura,transz+largura);
		glTexCoord2f(0, 0); glVertex3f(transx+comprimento,transy+altura,transz);
		glTexCoord2f(rep, 0); glVertex3f(transx+comprimento,transy,transz);
		glTexCoord2f(rep, rep); glVertex3f(transx+comprimento,transy,transz+largura);

		//cima

		glTexCoord2f(0, rep); glVertex3f(transx,transy+altura,transz);
		glTexCoord2f(0, 0); glVertex3f(transx,transy+altura,transz+largura);
		glTexCoord2f(rep, 0); glVertex3f(transx+comprimento,transy+altura,transz+largura);
		glTexCoord2f(rep, rep); glVertex3f(transx+comprimento,transy+altura,transz);


	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}
//------------------------------------------------------------------------------------------------------------------------

void looptry(GLfloat transx, GLfloat transy, GLfloat transz){
	glPushMatrix();
	glTranslatef(transx,transy,transz);
	glRotatef(ang,0,1,0);
	Lata();
	glPopMatrix();
	ang=ang+1;
	if(ang>=360)
		ang=1;
}

void jump(GLfloat transx, GLfloat transy, GLfloat transz,int maxjump){
	glPushMatrix();
	glTranslatef(transx,transy,transz);
	if(salto){
		if(saltoaltura>=maxjump){
			aux=1;
		}
		else if(saltoaltura<=0){
			aux=0;
		}
		if(aux==0){
			saltoaltura+=0.1;
		}
		else if(aux==1){
			saltoaltura-=0.1;
		}
		glTranslatef(0,saltoaltura,0);

	}
	glRotatef(-90,1,0,0);
	Lata();
	glPopMatrix();
}

void Reflection(){
	glEnable(GL_STENCIL_TEST);
	glColorMask(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//DESENHAR SUPERFÍCIE REFLECTORA
	ShowcaselowerTOP(0.2,0,0.2);

	glColorMask(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//DESENHAR OBJECTO REFLECTIDO
    glPushMatrix();
        glTranslatef(2.7,9,2.7);
        glScalef(1,-1,1);
        glRotatef(ang,0,1,0);
        Lata();
	glPopMatrix();
	ang=ang+1;
	if(ang>=360)
		ang=1;

	glDisable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);

	//DESENHAR SUPERFÍCIE REFLECTORA COM TRANSPARÊNCIA
	ShowcaselowerTOP(0.2,0,0.2);
	glDisable(GL_BLEND);

	Showcaselower(0.2,0,0.2);
	looptry(2.7,13,2.7);
}
//------------------------------------------------------------------------------------------------------------------------Damas

void LataDamas(int tipo){
	glEnable(GL_COLOR_MATERIAL);
	//-----------------------Corpo e topo
	glEnable(GL_TEXTURE_2D);
	if(tipo==1)
		glBindTexture(GL_TEXTURE_2D,texture[17]);
	else
		glBindTexture(GL_TEXTURE_2D,texture[18]);
	GLUquadricObj* s = gluNewQuadric ( );
	gluQuadricDrawStyle ( s, GLU_FILL );
	gluQuadricNormals ( s, GLU_SMOOTH );
	gluQuadricTexture ( s, GL_TRUE );
	glPushMatrix();//CORPO
		glTranslatef(0,0,0.5);
		gluCylinder(s,1,1,3,32,32);
	glPopMatrix();
	glPushMatrix();//TOPO circulo
		glTranslatef(0,0,4);
		gluDisk(s,0,0.9,32,32);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//---------------------Partes metalicas
	glEnable(GL_TEXTURE_2D);
	if(tipo==1)
		glBindTexture(GL_TEXTURE_2D,texture[19]);
	else
		glBindTexture(GL_TEXTURE_2D,texture[20]);
	gluQuadricDrawStyle ( s, GLU_FILL );
	gluQuadricNormals ( s, GLU_SMOOTH );
	gluQuadricTexture ( s, GL_TRUE );
	glPushMatrix();//BASE circulo
		gluDisk(s,0,0.9,32,32);
	glPopMatrix();
	glPushMatrix();//BASE Lata
		gluCylinder(s,0.9,1,0.5,32,32);
	glPopMatrix();
	glPushMatrix();//TOPO lata
		glTranslatef(0,0,3.5);
		gluCylinder(s,1,0.9,0.5,32,32);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}

void checkerboarbases(){
	//showcase
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f); glVertex3f(10,2,10);
		glTexCoord2f(10.0f,0.0f); glVertex3f(10,0,10);
		glTexCoord2f(10.0f,10.0f); glVertex3f(50,0,10);
		glTexCoord2f(0.0f,10.0f); glVertex3f(50,2,10);

		glTexCoord2f(0.0f,0.0f); glVertex3f(10,2,50);
		glTexCoord2f(10.0f,0.0f); glVertex3f(10,0,50);
		glTexCoord2f(10.0f,10.0f); glVertex3f(50,0,50);
		glTexCoord2f(0.0f,10.0f); glVertex3f(50,2,50);

		glTexCoord2f(0.0f,0.0f); glVertex3f(10,2,50);
		glTexCoord2f(10.0f,0.0f); glVertex3f(10,2,10);
		glTexCoord2f(10.0f,10.0f); glVertex3f(10,0,10);
		glTexCoord2f(0.0f,10.0f); glVertex3f(10,0,50);

		glTexCoord2f(0.0f,0.0f); glVertex3f(50,2,50);
		glTexCoord2f(10.0f,0.0f); glVertex3f(50,2,10);
		glTexCoord2f(10.0f,10.0f); glVertex3f(50,0,10);
		glTexCoord2f(0.0f,10.0f); glVertex3f(50,0,50);


	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_COLOR_MATERIAL);
	glNormal3f(0,1,0);

}


void checkerboardTOP(){
	int zvalue=10;
	int i,y,aux;
	for(i=0;i<8;i++){
		int xvalue=10;
		for(y=0;y<8;y++){
			if((y%2)==(i%2)){
				glMaterialfv(GL_FRONT, GL_AMBIENT, blackPlasticAmb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, blackPlasticDif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, blackPlasticSpec);
				glMaterialfv(GL_FRONT, GL_SHININESS, blackPlasticCoef);
			}
			else{
				glMaterialfv(GL_FRONT, GL_AMBIENT, whitePlasticAmb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, whitePlasticDif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, whitePlasticSpec);
				glMaterialfv(GL_FRONT, GL_SHININESS, whitePlasticCoef);
			}
			glBegin(GL_QUADS);
				glVertex3f(xvalue,2,zvalue);
				glVertex3f(xvalue+5,2,zvalue);
				glVertex3f(xvalue+5,2,zvalue+5);
				glVertex3f(xvalue,2,zvalue+5);
			glEnd();
			if (board[i][y] != 0){
				if (board[i][y] == 1){//Cokecans == BLACKS
					Checkerspos[i][y].x=xvalue;
					Checkerspos[i][y].z=zvalue;
					aux=1;
				}
				else if(board[i][y] == -1){//Pepsicans ==WHITES START FIRST
					Checkerspos[i][y].x=xvalue;
					Checkerspos[i][y].z=zvalue;
					aux=2;
				}
				glPushMatrix();
					glTranslatef(xvalue+2.5,2.1,zvalue+2.5);
					glRotatef(-90,1,0,0);
					LataDamas(aux);
				glPopMatrix();
			}
			xvalue=xvalue+5;
		}
		zvalue=zvalue+5;
	}
}


void checkersgame(){
	checkerboarbases();
	checkerboardTOP();//Board
}


int board_state(){
	int i,y;
	int blackcounter=0;
	int whitecounter=0;
	for(y=0;y<8;y++)
		if(board[0][y]==-1){
			whitecounter+=1;
		}
	for(y=0;y<8;y++)
		if(board[7][y]==1)
			blackcounter+=1;
	if(whitecounter>=4)
		return -1;
	else if(blackcounter>=4)
		return 1;
	else
		return 0;
}
//-------------------------------------------------------------------------------------------------------------



void showParticulas(Particle *particula, int sistema) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[22]);

    GLfloat mat[16];
    // ao usamos 3x3 que é a parte da rotaçao
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    GLfloat xleft, yleft, zleft, xright, yright, zright;

    for (int i=0;i<=MAX_PARTICULAS;i++){

        if(particula[i].y > 25)
            particula[i].life = 0.0f;
        if(particula[i].active) {
            glColor4f(1,1,1, particula[i].life);

            glGetFloatv(GL_MODELVIEW_MATRIX, mat);

            xleft = (-particula[i].size) * (mat[0] + mat[1]);;
            yleft = (-particula[i].size) * (mat[4] + mat[5]);
            zleft = (-particula[i].size) * (mat[8] + mat[9]);
            xright = (-particula[i].size) * (mat[0] - mat[1]);
            yright = (-particula[i].size) * (mat[4] - mat[5]);
            zright = (-particula[i].size) * (mat[8] - mat[9]);

            glBegin(GL_TRIANGLE_STRIP);
             glTexCoord2f(0.0,1.0); glVertex3f(particula[i].x-xright, particula[i].y-yright, particula[i].z-zright);     // top    right
             glTexCoord2f(0.0,0.0); glVertex3f(particula[i].x+xleft, particula[i].y+yleft, particula[i].z+zleft);     // top    left
             glTexCoord2f(1.0,1.0); glVertex3f(particula[i].x-xleft, particula[i].y-yleft, particula[i].z-zleft);     // bottom right
             glTexCoord2f(1.0,0.0); glVertex3f(particula[i].x+xright, particula[i].y+yright, particula[i].z+zright);     // bottom left
            glEnd();

            particula[i].x += particula[i].vx/(2.0*1000);
            particula[i].y += particula[i].vy/(2.0*1000);
            particula[i].z += particula[i].vz/(2.0*1000);
            particula[i].vx += particula[i].ax;
            particula[i].vy += particula[i].ay;
            particula[i].vz += particula[i].az;
            particula[i].life -= particula[i].fade;

            if (particula[i].life < 0.0f) {
				particula[i].life = 1.0f;
				particula[i].fade = float(rand()%100)/1000.0f+0.003f;
				if(sistema==1){
                    particula[i].x = 30.0f;
                    particula[i].y = 8.5f;
                    particula[i].z = 55.0f;
                }else if(sistema==2){
                    particula[i].x = 50.0f;
                    particula[i].y = 8.5f;
                    particula[i].z = 55.0f;
                }
                else{
                    particula[i].x = 10.0f;
                    particula[i].y = 8.5f;
                    particula[i].z = 55.0f;
                }
				particula[i].vx = float((rand()%60)-32.0f);
				particula[i].vy = float((rand()%60)-30.0f);
				particula[i].vz = float((rand()%60)-30.0f);
			}

        }
    }

    glDisable(GL_TEXTURE_2D);

}

void drawScene(){
	//Room
	room();

	baseFonte(0.2, 0, 25.3, 59.6, 2, 10, 21, 2);
	baseFonte(0.2, 0, 28.9, 59.6, 2, 10, 21, 2);
	baseFonte(0.2, 0, 26.3, 4, 5.2, 10, 21, 1);
	baseFonte(28, 0, 26.3, 4, 5.2, 10, 21, 1);
    baseFonte(2, 0, 26.3, 53, 5.2, 8.5, 22, 2);

	//Loop animation showcase
	Reflection();
	//Jump animation showcase
	jump(57.3,10,2.7,3);
	Showcaselower(54.8,0,0.2);
	ShowcaselowerTOP(54.8,0,0.2);
	ShowcaseUpper(54.8,0,0.2);

	showParticulas(particula1, 1);
    showParticulas(particula2, 2);
    showParticulas(particula3, 3);

	//CHECKERS
	checkersgame();


}


GLvoid resize(GLsizei width, GLsizei height)
{
	wScreen=width;
	hScreen=height;
	drawScene();
}



void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	if(gamecamera){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(100.0, wScreen/hScreen, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt( 30, 18.8,30, 30, 2, 30, 0, 0,-1);
	}
	else{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(100.0, wScreen/hScreen, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);//Observador movel
	}

	drawScene();

	glutSwapBuffers();
	glutPostRedisplay();
}





//======================================================= EVENTOS

//==========================================  Functions

void updateVisao(){
	obsT[0] =obsP[0]+rVisao*cos(aVisao);
	obsT[2] =obsP[2]-rVisao*sin(aVisao);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
	case '1':
	   salto=!salto;
	   glutPostRedisplay();
	break;


	case 'W':
	case 'w':
		obsP[1] = (obsP[1]+ 0.1) ;
		if(obsP[1]>=39.8)
			obsP[1]=39.8;
		glutPostRedisplay();
	break;

	case 'S':
	case 's':
		obsP[1] = (obsP[1]- 0.1) ;
		if(obsP[1]<=0.2)
			obsP[1]=0.2;
		glutPostRedisplay();
	break;

	case 'Q':
	case 'q':
		obsT[1]=obsT[1] + 0.1;
		if(obsT[1]>=39.8)
			obsT[1]=39.8;
		obsP[1]=obsP[1]+ 0.1;
		if(obsP[1]>=39.8)
			obsP[1]=39.8;
		glutPostRedisplay();
	break;

	case 'A':
	case 'a':
		 obsT[1]=obsT[1] - 0.1;
		 if(obsT[1]<=0.2)
			obsT[1]=0.2;
		obsP[1]=obsP[1] - 0.1;
		if(obsP[1]<=0.2)
			obsP[1]=0.2;
		glutPostRedisplay();
	break;

	case 'E':
	case 'e':
	   obsT[1]=obsT[1] + 0.1;
	   if(obsT[1]>=39.8)
			obsT[1]=39.8;
	   glutPostRedisplay();
	break;

	case 'D':
	case 'd':
		obsT[1]=obsT[1] - 0.1;
		 if(obsT[1]<=0.2)
			obsT[1]=0.2;
	   glutPostRedisplay();
	break;

	case 'R':
	case 'r':
		roomtex=!roomtex;
		if (roomtex){
			printf("Sala madeira e luzes ativas\n");
			glEnable(GL_LIGHTING);
		}
		else{
			glDisable(GL_LIGHTING);
			printf("Sala pedra e luzes desativadas\n");
		}
		glutPostRedisplay();
	break;

	case 'F':
	case 'f':
		foco=!foco;
		if (foco){
			glEnable(GL_LIGHT1);
			printf("Foco activado\n");
		}
		else{
			glDisable(GL_LIGHT1);
			printf("Foco desactivado\n");
		}
		glutPostRedisplay();
	break;

	case 'G':
	case 'g':
		gamecamera=!gamecamera;
		if (gamecamera){
			printf("Gamecam activado\n");
		}
		else{
			printf("Gamecam desactivado\n");
		}
		glutPostRedisplay();
	break;

	case 'I':
	case 'i':
		pontual=!pontual;
		if (pontual){
			glEnable(GL_LIGHT0);
			printf("Luz Pontual activada\n");
		}
		else{
			glDisable(GL_LIGHT0);
			printf("Luz pontual desactivada\n");
		}
		glutPostRedisplay();
	break;
	//TESTE
	case 'T':
	case 't':
	printf("%f %f %f\n",obsP[0],obsP[1],obsP[2]);
		printf("%f %f %f\n",obsT[0],obsT[1],obsT[2]);
	break;




	//--------------------------- Escape
	case 27:
		exit(0);
	break;

  }
}


void teclasNotAscii(int key, int x, int y)
{
	if(key == GLUT_KEY_UP) {
		obsP[0]=obsP[0]+incVisao*cos(aVisao);
		obsP[2]=obsP[2]-incVisao*sin(aVisao);

	}
	if(key == GLUT_KEY_DOWN) {
		obsP[0]=obsP[0]-incVisao*cos(aVisao);
		obsP[2]=obsP[2]+incVisao*sin(aVisao);

	}
	if(key == GLUT_KEY_LEFT) {
		aVisao = (aVisao + 0.1) ;
	}
	if(key == GLUT_KEY_RIGHT) {
		aVisao = (aVisao - 0.1) ;
	}
	if(obsP[0]<=0.5)
		obsP[0] = 0.5;
	else if(obsP[0]>=59.5)
		obsP[0] = 59.5;
	if(obsP[2]<=0.5)
		obsP[2] = 0.5;
	else if(obsP[2]>=59.5)
		obsP[2] = 59.5;
	updateVisao();
}

void Mouse(int btn, int state, int x, int y){
	int winner;
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && gamecamera)
	{
		if(gamecamera){
			int row = y / 90;
			int col = x / 100;
			if (selected){
				if (turn == 1){
					if (((row-sRow) > 1) && (abs(sCol - col) > 1)){
						if (sCol - col > 0){
							if (board[sRow + 1][sCol - 1] == 0){
								selected = false;
								return;
							}
							else {
								board[row][col] = board[sRow][sCol];
								board[sRow][sCol] = 0;
								board[sRow + 1][sCol - 1] = 0;
								turn *= -1;
								selected = false;
								glutPostRedisplay();
								return;
							}
						}
						else if (sCol - col < 0){
							if (board[sRow + 1][sCol + 1] == 0){
								selected = false;
								return;
							}
							else {
								board[row][col] = board[sRow][sCol];
								board[sRow][sCol] = 0;
								board[sRow + 1][sCol + 1] = 0;
								turn *= -1;
								selected = false;
								glutPostRedisplay();
								return;
							}
						}
					}
					if ((sRow >= row) || (abs(sRow-row)>1) || (abs(sCol-col)>1) || (sCol==col)){
						selected = false;
						return;
					}
				}
				else if(turn == -1){
					if (((sRow - row) > 1) && (abs(sCol - col) > 1)){
						if (sCol - col > 0){
							if (board[sRow - 1][sCol - 1] == 0){
								selected = false;
								return;
							}
							else {
								board[row][col] = board[sRow][sCol];
								board[sRow][sCol] = 0;
								board[sRow - 1][sCol - 1] = 0;
								turn *= -1;
								selected = false;
								glutPostRedisplay();
								return;
							}
						}
						else if (sCol - col < 0){
							if (board[sRow - 1][sCol + 1] == 0){
								selected = false;
								return;
							}
							else {
								board[row][col] = board[sRow][sCol];
								board[sRow][sCol] = 0;
								board[sRow - 1][sCol + 1] = 0;
								turn *= -1;
								selected = false;
								glutPostRedisplay();
								return;
							}
						}
					}

					if ((sRow <= row) || (abs(sRow - row) > 1) || (abs(sCol - col) > 1) || (sCol == col)){
						selected = false;
						return;
					}
				}

				if (board[row][col] == 0){
					board[row][col] = board[sRow][sCol];
					board[sRow][sCol] = 0;
				}
				selected = false;
				turn *= -1;
			}
			else {
				if (board[row][col] == turn){
					selected = true;
					sRow = row;
					sCol = col;
					printf("Checker Found at: ");
				}
				printf("%i %i\n", row, col);
			}
			winner=board_state();
			if(winner==-1){
				printf("PEPSI WINNER\n");
				gamecamera =0;
				turn=-1;
				selected = false;
				sRow = 0;
				sCol = 0;
				turn = -1;
				InitBoard();
				drawScene();
			}
			else if(winner==1){
				printf("COLA WINNER\n");
				gamecamera =0;
				turn=1;
				selected = false;
				sRow = 0;
				sCol = 0;
				turn = -1;
				InitBoard();
				drawScene();
			}
			glutPostRedisplay();
		}
		else
		printf("Clicar tecla G para activar gamecamera para poder ser jogado!!!\n");
	}
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//RESETS SCENE
	{
		gamecamera =0;
		selected = false;
		sRow = 0;
		sCol = 0;
		turn = -1;
		InitBoard();
		drawScene();
	}
}



//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen);
	glutInitWindowPosition (400, 100);
	glutCreateWindow ("No Rules Checkers");

	init();
	glutSpecialFunc(teclasNotAscii);
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(Mouse);

	glutMainLoop();

	return 0;
}




