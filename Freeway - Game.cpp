#include <iostream>
#include <cstdlib>
#include <fstream>
#include <GL/glut.h>
#include <sstream>
#include <time.h>
#include "fmod.hpp" //fmod c++ header
#pragma comment( lib, "fmodex_vc.lib" ) // fmod library


#define CHICKEN1_TEX 0
#define CHICKEN2_TEX 1
#define CHICKEN_FACTOR 10.0
#define CHICKEN_SIZE_X CHICKEN_FACTOR/9
#define CHICKEN_SIZE_Y CHICKEN_FACTOR/23
#define CAR1_TEX 2
#define CAR2_TEX 3
#define CAR3_TEX 4
#define CAR4_TEX 5
#define P1_TEX 6
#define P2_TEX 7
#define P1WINNER_TEX 8 
#define P2WINNER_TEX 9
#define DRAW_TEX 10
#define TITLE_TEX 11
#define INSTRUCTIONS_TEX 12

using namespace std;
using std::stringstream;

//Global variables

GLint movecar1 = 0; //used to move car 1
GLint movecar2 = 0; //used to move car 2
GLint movecar3 = 0; //used to move car 3
GLint movecar4 = 0; //used to move car 4
GLint moveplayer1x=0; //used to move p1 in x
GLint moveplayer1y=0; //used to move p1 in y
GLint moveplayer2x=0; //used to move p2 in x
GLint moveplayer2y=0; //used to move p2 in y
GLint scene=0; //used to change the scene
GLint inst_scene = 0; //scene
char* inst_path = "Textures/instructions.bmp"; //instrction's image path
int score_p1 = 0; //score of player 1
int score_p2 = 0; //score of player 2
static GLsizei width, height;
time_t start, ending; //start and ending time
tm * ptm;
float color_r = 1.0, color_g = 1.0, color_b = 1.0;
static unsigned int texture[13]; //Array of texture indices
int playsound = 1;


// Rountines for bitmap loading
struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
  
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
 
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4); 
   
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   { 
      temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
   }

   return bmp;
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, const char *string)
{  
   const char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// |-----------------------------------------------------------------------------------------------------------|
// |------------------------------------------------TEXTURES---------------------------------------------------|
// |-----------------------------------------------------------------------------------------------------------|

//Player 1's texture - white chicken
void loadPlayer1Texture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/chicken.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[CHICKEN1_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index.
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Player 2's texture - black chicken
void loadPlayer2Texture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/chicken2.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[CHICKEN2_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Car 1's texture
void loadCar1Texture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/car1.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[CAR1_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Car 2's texture
void loadCar2Texture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/car2.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[CAR2_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Car 3's texture
void loadCar3Texture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/car3.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[CAR3_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Car 4's texture
void loadCar4Texture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/car4.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[CAR4_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Player 1 - image
void loadP1MenuTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/p1.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[P1_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Player 2 - image
void loadP2MenuTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/p2.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[P2_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Texture when p1 is the winner
void loadPlayer1WinnerTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/p1winner.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[P1WINNER_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Texture when p2 is the winner
void loadPlayer2WinnerTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/p2winner.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[P2WINNER_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Texture when is a draw
void loadDrawTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/draw.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[DRAW_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Game Title
void loadTitleTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData("Textures/title.bmp");

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[TITLE_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

//Instructions
void loadInstructionsTexture()
{
	BitMapFile *image[1];
   
   // Load the textures.
   image[0] = getBMPData(inst_path);

   // Activate texture index texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[INSTRUCTIONS_TEX]); 

   // Set texture parameters for wrapping.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   // Set texture parameters for filtering.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // Specify an image as the texture to be bound with the currently active texture index
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}

// |-----------------------------------------------------------------------------------------------------------|
// |------------------------------------------------DRAWING OBJECTS--------------------------------------------|
// |-----------------------------------------------------------------------------------------------------------|

//Drawing the speedway
void speedWay() {
	glColor3f(1.0,1.0,1.0);        
    glLineWidth(3);
    glBegin(GL_LINES);
	glVertex2i(1,1);
	glVertex2i(29,1);
	glVertex2i(1,10);
	glVertex2i(29,10);
    glEnd();
        
    glColor3f(1.0,1.0,1.0);        
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(2,3);
	glVertex2i(6,3);
	glVertex2i(10,3);
	glVertex2i(14,3);
	glVertex2i(18,3);
	glVertex2i(22,3);
	glVertex2i(26,3);
	glVertex2i(29,3);
	glVertex2i(2,8);
	glVertex2i(6,8);
	glVertex2i(10,8);
	glVertex2i(14,8);
	glVertex2i(18,8);
	glVertex2i(22,8);
	glVertex2i(26,8);
	glVertex2i(29,8);
	glColor3f(1.0,1.0,0.0);
	glVertex2i(2,5);
	glVertex2i(6,5);
	glVertex2i(10,5);
	glVertex2i(14,5);
	glVertex2i(18,5);
	glVertex2i(22,5);
	glVertex2i(26,5);
	glVertex2i(29,5);
	glVertex2i(2,6);
	glVertex2i(6,6);
	glVertex2i(10,6);
	glVertex2i(14,6);
	glVertex2i(18,6);
	glVertex2i(22,6);
	glVertex2i(26,6);
	glVertex2i(29,6);
	glEnd();
}

//Drawing player 1
void player1() {         
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[CHICKEN1_TEX]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0); glVertex2f(moveplayer1x+15-CHICKEN_SIZE_X, moveplayer1y+1-CHICKEN_SIZE_Y);
		glTexCoord2f(1.0, 0.0); glVertex2f(moveplayer1x+15+CHICKEN_SIZE_X, moveplayer1y+1-CHICKEN_SIZE_Y);
		glTexCoord2f(1.0, 1.0); glVertex2f(moveplayer1x+15+CHICKEN_SIZE_X, moveplayer1y+1+CHICKEN_SIZE_Y);
		glTexCoord2f(0.0, 1.0); glVertex2f(moveplayer1x+15-CHICKEN_SIZE_X, moveplayer1y+1+CHICKEN_SIZE_Y);
	glEnd();
	glDisable(GL_TEXTURE_2D);       
}

//Drawing player 2
void player2() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[CHICKEN2_TEX]);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0); glVertex2f(moveplayer2x+15-CHICKEN_SIZE_X, moveplayer2y+1-CHICKEN_SIZE_Y);
		glTexCoord2f(1.0, 0.0); glVertex2f(moveplayer2x+15+CHICKEN_SIZE_X, moveplayer2y+1-CHICKEN_SIZE_Y);
		glTexCoord2f(1.0, 1.0); glVertex2f(moveplayer2x+15+CHICKEN_SIZE_X, moveplayer2y+1+CHICKEN_SIZE_Y);
		glTexCoord2f(0.0, 1.0); glVertex2f(moveplayer2x+15-CHICKEN_SIZE_X, moveplayer2y+1+CHICKEN_SIZE_Y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Drawing car 1
void car1(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[CAR1_TEX]);
    glBegin(GL_QUADS);
  		glTexCoord2f(0.0, 0.0); glVertex2f (movecar1+1,1.5);
  		glTexCoord2f(1.0, 0.0); glVertex2f (movecar1+1,2.5);
		glTexCoord2f(1.0,1.0); glVertex2f (movecar1+5,2.5);
		glTexCoord2f(0.0, 1.0); glVertex2f (movecar1+5,1.5);
    glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Drawing car 2
void car2() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[CAR2_TEX]);
    glBegin(GL_QUADS);
  		glTexCoord2f(0.0, 0.0); glVertex2f(movecar2+1,3.5);
  		glTexCoord2f(1.0, 0.0); glVertex2f(movecar2+1,4.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(movecar2+5,4.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(movecar2+5,3.5);
    glEnd();	
	glDisable(GL_TEXTURE_2D);
}

//Drawing car 3
void car3(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[CAR3_TEX]);
    glBegin(GL_QUADS);
  		glTexCoord2f(0.0, 0.0); glVertex2f (movecar3+1,6.5);
  		glTexCoord2f(1.0, 0.0); glVertex2f (movecar3+1,7.5);
		glTexCoord2f(1.0, 1.0); glVertex2f (movecar3+5,7.5);
		glTexCoord2f(0.0, 1.0); glVertex2f (movecar3+5,6.5);
    glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Drawing car 4
void car4(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[CAR4_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(movecar4+1,8.5);
		glTexCoord2f(1.0, 0.0); glVertex2f(movecar4+1,9.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(movecar4+5,9.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(movecar4+5,8.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Drawing player 1's menu
void menuP1() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[P1_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(8,11.5);
  		glTexCoord2f(1.0, 0.0); glVertex2f(17,11.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(17,12.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(8,12.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Drawing player 2's menu
void menuP2() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[P2_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(8,11.5);
  		glTexCoord2f(1.0, 0.0); glVertex2f(17,11.5);
		glTexCoord2f(1.0, 1.0); glVertex2f(17,12.5);
		glTexCoord2f(0.0, 1.0); glVertex2f(8,12.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Drawing when p1 is the winner
void p1Winner() {
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[P1WINNER_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
  		glTexCoord2f(1.0, 0.0); glVertex2f(30,0);
		glTexCoord2f(1.0, 1.0); glVertex2f(30,13);
		glTexCoord2f(0.0, 1.0); glVertex2f(0,13);
	glEnd();
	glDisable(GL_TEXTURE_2D);	
}

//Drawing when p2 is the winner
void p2Winner() {
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[P2WINNER_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
  		glTexCoord2f(1.0, 0.0); glVertex2f(30,0);
		glTexCoord2f(1.0, 1.0); glVertex2f(30,13);
		glTexCoord2f(0.0, 1.0); glVertex2f(0,13);
	glEnd();
	glDisable(GL_TEXTURE_2D);	
}

//Drawing when it's a draw
void draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[DRAW_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
  		glTexCoord2f(1.0, 0.0); glVertex2f(30,0);
		glTexCoord2f(1.0, 1.0); glVertex2f(30,13);
		glTexCoord2f(0.0, 1.0); glVertex2f(0,13);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

//Drawing the title
void title() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[TITLE_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
  		glTexCoord2f(1.0, 0.0); glVertex2f(30,0);
		glTexCoord2f(1.0, 1.0); glVertex2f(30,13);
		glTexCoord2f(0.0, 1.0); glVertex2f(0,13);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

//Drawing the instructions
void instructions() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[INSTRUCTIONS_TEX]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2f(0,0);
  		glTexCoord2f(1.0, 0.0); glVertex2f(30,0);
		glTexCoord2f(1.0, 1.0); glVertex2f(30,13);
		glTexCoord2f(0.0, 1.0); glVertex2f(0,13);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

// |-----------------------------------------------------------------------------------------------------------|
// |------------------------------------------------ANIMATIONS-------------------------------------------------|
// |-----------------------------------------------------------------------------------------------------------|

//Moving the cars, in which one has its own speed
//Function to move the car 1
void movingCar1(int value) {
	movecar1 += 4;
	if (movecar1 == 44)
		movecar1 = 0;
	glutPostRedisplay();
	glutTimerFunc(60,movingCar1,1);
}

//Function to move the car 2
void movingCar2(int value) {
	movecar2 += 4;
	if(movecar2 == 44)
		movecar2 = 0;
	glutPostRedisplay();
	glutTimerFunc(60,movingCar2,1);
}

//Function to move the car 3
void movingCar3(int value) {
	movecar3 -= 4;
	if(movecar3 == -8)
		movecar3 = 32;
	glutPostRedisplay();
	glutTimerFunc(60,movingCar3,1);
}

//Function to move the car 4
void movingCar4(int value) {
	movecar4 -= 4;
	if(movecar4 == -8)
		movecar4 = 40;
	glutPostRedisplay();
	glutTimerFunc(60,movingCar4,1);
}

//Counting the elapsed time
int timer(){
time (&ending);
time_t timeElapsed = ending - start;
ptm = gmtime ( &timeElapsed );
int seconds = ptm->tm_sec;
return seconds;
}

//Blinks the score when the time is running out
void timeRunningOut(int value) {
	if (timer() > 45) {
		if (color_g == 1.0) {
			color_g = 0.0;
			color_b = 0.0;
		}
		else {
		color_g = 1.0;
		color_b = 1.0;
		}
	}
	glutTimerFunc(100,timeRunningOut,1);
}

//Changing the instruction scene
void changeInstructionScene(int value) {
	if(inst_scene == 1) 
		inst_path = "Textures/getready.bmp";
	if(inst_scene == 2)
		inst_path = "Textures/3.bmp";
	if(inst_scene == 3)
		inst_path = "Textures/2.bmp";
	if(inst_scene == 4)
		inst_path = "Textures/1.bmp";
	if(inst_scene == 5)
		inst_path = "Textures/go.bmp";
	if(inst_scene == 6)
		scene = 2;
	loadInstructionsTexture();
	inst_scene++;
	glutTimerFunc(1600,changeInstructionScene,1);
}

// |-----------------------------------------------------------------------------------------------------------|
// |------------------------------------------------COLLISION--------------------------------------------------|
// |-----------------------------------------------------------------------------------------------------------|

//Function to detect the collision of Player 1 with the cars and if it happens,
// the player goes back to the bottom
void collisionPlayer1() {
	if ((movecar1-10 == moveplayer1x+1) && (2 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar1-10 == moveplayer1x+2) && (2 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar1-10 == moveplayer1x+3) && (2 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar1-10 == moveplayer1x+4) && (2 == moveplayer1y+1)) moveplayer1y = 0;

    if ((movecar2-10 == moveplayer1x+1) && (4 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar2-10 == moveplayer1x+2) && (4 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar2-10 == moveplayer1x+3) && (4 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar2-10 == moveplayer1x+4) && (4 == moveplayer1y+1)) moveplayer1y = 0;

    if ((movecar2-10 == moveplayer1x+1) && (7 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar2-10 == moveplayer1x+2) && (7 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar2-10 == moveplayer1x+3) && (7 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar2-10 == moveplayer1x+4) && (7 == moveplayer1y+1)) moveplayer1y = 0;

	if ((movecar4-10 == moveplayer1x+1) && (9 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar4-10 == moveplayer1x+2) && (9 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar4-10 == moveplayer1x+3) && (9 == moveplayer1y+1)) moveplayer1y = 0;
    if ((movecar4-10 == moveplayer1x+4) && (9 == moveplayer1y+1)) moveplayer1y = 0;

	if ((10 == moveplayer1y+1) && (scene == 2)) { score_p1++; moveplayer1y = 0;}
	//If the time is 59 seconds, changes the scene
    if (timer() == 59) scene = 3;
	glutPostRedisplay();
}

//Function to detect the collision of Player 2 with the cars and if it happens,
// the player goes back to the bottom
void collisionPlayer2(){
	if ((movecar1-10 == moveplayer2x+1) && (2 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar1-10 == moveplayer2x+2) && (2 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar1-10 == moveplayer2x+3) && (2 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar1-10 == moveplayer2x+4) && (2 == moveplayer2y+1)) moveplayer2y = 0;

    if ((movecar2-10 == moveplayer2x+1) && (4 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar2-10 == moveplayer2x+2) && (4 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar2-10 == moveplayer2x+3) && (4 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar2-10 == moveplayer2x+4) && (4 == moveplayer2y+1)) moveplayer2y = 0;

    if ((movecar2-10 == moveplayer2x+1) && (7 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar2-10 == moveplayer2x+2) && (7 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar2-10 == moveplayer2x+3) && (7 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar2-10 == moveplayer2x+4) && (7 == moveplayer2y+1)) moveplayer2y = 0;

	if ((movecar4-10 == moveplayer2x+1) && (9 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar4-10 == moveplayer2x+2) && (9 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar4-10 == moveplayer2x+3) && (9 == moveplayer2y+1)) moveplayer2y = 0;
    if ((movecar4-10 == moveplayer2x+4) && (9 == moveplayer2y+1)) moveplayer2y = 0;

	if ((10 == moveplayer2y+1) && (scene == 2)) { score_p2++; moveplayer2y = 0; }            
    //If the time is 59 seconds, changes the scene
	if (timer() == 59) scene = 3;
	glutPostRedisplay();	
}
// |-----------------------------------------------------------------------------------------------------------|
// |------------------------------------------------SOUND------------------------------------------------------|
// |-----------------------------------------------------------------------------------------------------------|

void playSound(){
	
    FMOD::System     *system; //handle to FMOD engine
    FMOD::Sound      *sound1; //sound that will be loaded and played
    //init FMOD
	FMOD::System_Create(&system);// create an instance of the game engine
	system->init(32, FMOD_INIT_NORMAL, 0);// initialise the game engine with 32 channels

	//load sounds
	system->createSound("Sounds/trafficpass.wav", FMOD_HARDWARE, 0, &sound1);
	sound1->setMode(FMOD_LOOP_NORMAL);    
			
	FMOD::Channel *channel;
	system->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);
}
//---------------------------------------------------------------------------------------------------------------------

//The drawing scene function
void drawScene(void)
{ 
	timer();
	stringstream score, score2; //defining a string

	if (scene == 0) { 
		glViewport(0, 0, width,  height);
		title();
		glutSwapBuffers();
	}
	if (scene == 1) { 
		glViewport(0, 0, width,  height);
		instructions();
		glutSwapBuffers();
	}
    if(scene == 2) 
    {   
		
		glClear(GL_COLOR_BUFFER_BIT);
		if(playsound) {
			playSound(); //Play the sound
			playsound = 0;
		}
		// |-----------------------------------------------------------------------------------------------------------|
		// |------------------------------------------------PLAYER 1---------------------------------------------------|
		// |-----------------------------------------------------------------------------------------------------------|
		glViewport(0, 0, width/2.0,  height); //defining the viewport
		//Calling the objects
		speedWay();
		collisionPlayer1();
		car1();
		car2();
		car3();
		car4();
		player1();
		//Writing the score
		glColor3f(color_r,color_g,color_b);
		glRasterPos3f(8.0, 11.0, 0.0);
		score << "Score: " << score_p1 << ends; 
		writeBitmapString(GLUT_BITMAP_HELVETICA_18, score.str().c_str());
		menuP1();
		glColor3f(0.0,0.0,0.0);
		// |-----------------------------------------------------------------------------------------------------------|
		// |------------------------------------------------PLAYER 2---------------------------------------------------|
		// |-----------------------------------------------------------------------------------------------------------|
		glViewport(width/2.0, 0, width/2.0, height); //defining the viewport
		//Calling the objects
		speedWay();
		collisionPlayer2();
		car1();
		car2();
		car3();
		car4();
		player2();
		glColor3f(color_r,color_g,color_b);
		//Writing the score
		glRasterPos3f(8.0, 11.0, 0.0);
		score2 << "Score: " << score_p2 << ends;
		writeBitmapString(GLUT_BITMAP_HELVETICA_18, score2.str().c_str());
		menuP2();
		glColor3f(0.0,0.0,0.0);
		glLineWidth(2.0);
		glBegin(GL_LINES);
		glVertex3f(0.1, 0.0, 0.0);
		glVertex3f(0.1, 13.0, 0.0);
		glEnd();
		glLineWidth(1.0);
		glutSwapBuffers();	
    }
	
	//Winning scene
	if(scene == 3) 
    {
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glRasterPos3f(12.0, 6.0, 0.0);
			if (score_p1 > score_p2) 
				p1Winner();
			else if (score_p2 > score_p1)
				p2Winner();
			else
				draw();
				//writeBitmapString(GLUT_BITMAP_HELVETICA_18, "Draw!");
		glutSwapBuffers();  
    }  
      
}

// |-----------------------------------------------------------------------------------------------------------|
// |------------------------------------------------INPUTS-----------------------------------------------------|
// |-----------------------------------------------------------------------------------------------------------|

//Player 1 input to move 
void specialKeyInput(int key, int x, int y) {
	if(key == GLUT_KEY_UP)      
		moveplayer2y += 1; 
	if(key == GLUT_KEY_DOWN)   
		moveplayer2y -= 1; 
	if(key == GLUT_KEY_LEFT)    
		moveplayer2x -= 1; 
	if(key == GLUT_KEY_RIGHT)   
		moveplayer2x += 1; 

    if (moveplayer2y < 0)   		 
		moveplayer2y = 0; 
	if (moveplayer2y > 9)
		moveplayer2y = 9;  
    if (moveplayer2x < -13)
		moveplayer2x = -13;
    if (moveplayer2x > 13) 
		moveplayer2x = 13;
}

//Player 2 input to move
void keyInput(unsigned char key, int x, int y){
	if(key == 27)  
		exit(0);
	if(key == ' '){
		if(scene == 0)
		    scene++;
	}
	if(key == 119) 
		moveplayer1y += 1;
	if(key == 115)
		moveplayer1y -= 1;
	if(key == 100)
		moveplayer1x += 1;
	if(key == 97)
		moveplayer1x -= 1;
	
	if (moveplayer1y < 0)   		 
		moveplayer1y = 0; 
	if (moveplayer1y > 9)
		moveplayer1y = 9;  
    if (moveplayer1x < -13)
		moveplayer1x = -13;
    if (moveplayer1x > 13) 
		moveplayer1x = 13;
}

//--------------------------------------------------------------------------------------------------------------------

//Setting the background color to be gray
void setup() {
	time (&start);
   	glClearColor(0.75, 0.75, 0.75, 0.75);
	
	// Texture loading functions.
   glGenTextures(13, texture);
   loadPlayer1Texture();
   loadPlayer2Texture();
   loadCar1Texture();
   loadCar2Texture();
   loadCar3Texture();
   loadCar4Texture();
   loadP1MenuTexture();
   loadP2MenuTexture();
   loadPlayer1WinnerTexture();
   loadPlayer2WinnerTexture();
   loadDrawTexture();
   loadTitleTexture();
   loadInstructionsTexture();
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

//Resize function
void resize(GLsizei w, GLsizei h){

	if(h==0) h=1;

	width = w;
	height = h;
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width <= height) 
		gluOrtho2D (0.0f, 23.0f, 0.0f*height/width, 13.0f*height/width);
	else 
		gluOrtho2D (0.0f*width/height, 23.0f*width/height, 0.0f, 13.0f);

}

//Main function
int main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize (690, 530); 
    glutInitWindowPosition (50, 50);  
	glutCreateWindow("Freeway");
	glutKeyboardUpFunc(keyInput); //Keyboard Up to avoid the situation that the player wants to hold the keyInput
    glutSpecialUpFunc(specialKeyInput);  
	setup(); 
	//playSound(); //Play the sound
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize); 
    glutTimerFunc(10,movingCar1,1);  //Animations
    glutTimerFunc(10,movingCar2,1);   
    glutTimerFunc(10,movingCar3,1);  
	glutTimerFunc(10,movingCar4,1);
	glutTimerFunc(10,timeRunningOut,1);
	glutTimerFunc(1700,changeInstructionScene,1);
	glutMainLoop();
}