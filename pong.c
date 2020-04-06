#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_graph2.h"
#include <stdlib.h>


FastSPRITE TheBall;						// Definining the ball Sprite Structure
FastSPRITE CpuPad;						// Defining CPU Pad Structure
FastSPRITE PlyPad;						// Defining Player Pad Structure




// Function definitions
void DrawSprite(void);
void BallCal(void);
void ComputerCal(void);
void GameStart(void);

//Global Vars
signed char DirX;						// Direction of the ball
signed char DirY;						// 

char PlyScore,CpuScore;


const char PatternBall[]={
						0b11100000,
						0b11100000,
						0b11100000,
						0b00000000,
						0b00000000,
						0b00000000,
						0b00000000,
						0b00000000
};

const char PatternPad[]={
						0b11100000,
						0b11100000,
						0b11100000,
						0b11100000,
						0b11100000,
						0b11100000,
						0b11100000,
						0b11100000
};


const signed char moves[9]={0,-1,0,0,0,1,0,0,0}; 


void GameStart(void)
{
	char Temp[5];

	SetColor(255);
	PutText((256-20*8)/2,4,"Press SPACE to START",0);

	// Initial Positions
	PlyPad.x=15;
	PlyPad.y=100;
	CpuPad.x=240;
	CpuPad.y=100;
	TheBall.x=128;
	TheBall.y=100;

	DirX*=-1;

	DrawSprite();

	Itoa(PlyScore,Temp,10);
	PutText(10,4,Temp,0);

	Itoa(CpuScore,Temp,10);
	PutText(235,4,Temp,0);	

	WaitKey();
	PutText((256-20*8)/2,4,"                    ",0);
}


// Put all sprite on. screen
void DrawSprite(void)
{
		// Check Sprite Colision
		if (SpriteCollision() && TheBall.x>235 && DirX>0)		// Collision with CPU Pad
			DirX*=-1;

		if (SpriteCollision() && TheBall.x<15 && DirX<0)		// Collision with Player Pad
			DirX*=-1;

		fPutSprite(&PlyPad);		// Use the Fast Sprite Function
		fPutSprite(&CpuPad);
		fPutSprite(&TheBall);

		// Check Ball Outside Game field
		if (TheBall.x<10)				// Player is on The Left Side
		{
			CpuScore++;
			GameStart();
		}
		if (TheBall.x>245)
		{
			PlyScore++;
			GameStart();
		}
}


// Ball Position 
void BallCal(void)
{
	TheBall.x+=DirX;
	TheBall.y+=DirY;

	if (TheBall.y>=210 || TheBall.y<5)		// Douncing the Ball on the Top and Bottom border 
		DirY*=-1;

	

}

void ComputerCal(void)
{
		CpuPad.y=TheBall.y;				// Simple Algorythm ! Cpu Cannot be beaten !
}

// Wait Routine
void FT_Wait(int cicles) {
  unsigned int i;

  for(i=0;i<cicles;i++)
  {
    while(Vsynch()==0)
    {}
  }
}


void main (void)
{
	
	char joy;


	SetColors(15,0,0);
	Screen(5);
	Sprite8();
	SpriteDouble();
	KeySound(0);

	SetSpritePattern(0, PatternBall,8);
	SetSpritePattern(1, PatternPad,8);


	// Defining Variables
	// Player Pad Sprite
	PlyPad.x=15;
	PlyPad.y=100;
	PlyPad.spr=1;
	PlyPad.pat=1;

	PlyScore=0;

	// Cpu Pad Sprite
	CpuPad.x=240;
	CpuPad.y=100;
	CpuPad.spr=2;
	CpuPad.pat=1;

	CpuScore=0;

	// Ball Sprite
	TheBall.x=128;
	TheBall.y=100;
	TheBall.spr=0;
	TheBall.pat=0;

	DirX=1;
	DirY=1;


	GameStart();

	while (Inkey()!=27) 				// Main loop
	{

		joy=JoystickRead(0);		// Reading Joystick 0 (Keyboard)

		PlyPad.y+=moves[joy];		// Update the Y position of the Player Pad
		BallCal();
		ComputerCal();
		DrawSprite();

		FT_Wait(170);
	}



	// Ending program, and return to DOS
	Screen(0);
	KeySound(1);
	Exit(0);




}