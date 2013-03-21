#include <stdio.h>
#include <allegro5\allegro.h>

#include "CBlock.h"

void CBlock::Define(char** d)
{
	numRotations = 1;
	rotation = 0;

	def = new char**[4];
	def[0] = d;
}

void CBlock::Init()
{
	x = 3;
	y = 0;

	canRotate = true;
	canMoveLeft = true;
	canMoveRight = true;
	canMoveDown = true;

	time = al_get_time();
}

void CBlock::DefineRotation(char** d)
{
	numRotations = 2;

	def[1] = d;
}

void CBlock::DefineRotation(char** d1, char** d2, char** d3)
{
	numRotations = 4;

	def[1] = d1;
	def[2] = d2;
	def[3] = d3;
}

void CBlock::SetColor(int c)
{
	for(int n = 0; n < numRotations; n++) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(def[n][i][j] != 0)
					def[n][i][j] = c;
			}
		}
	}
}

int CBlock::GetX()
{
	return x;
}

int CBlock::GetY()
{
	return y;
}

void CBlock::Rotate()
{
	rotation += 1;
	if(rotation >= numRotations)
		rotation = 0;
}

void CBlock::MoveLeft()
{
	x -= 1;
}

void CBlock::MoveRight()
{
	x += 1;
}

void CBlock::MoveDown()
{
	y += 1;
}

char** CBlock::GetDef()
{
	return def[rotation];
}

char** CBlock::GetRotDef()
{
	int tempRot = rotation + 1;
	if(tempRot >= numRotations)
		tempRot = 0;

	return def[tempRot];
}

double CBlock::GetTimeAlive()
{
	return (al_get_time() - time);
}

void CBlock::Cleanup()
{
	for(int i = 0; i < numRotations; i++) {
		for(int j = 0; j < 4; j++)
			delete[] def[i][j];
		delete[] def[i];
	}

	if(numRotations > 0)
		delete[] def;

	printf("Block cleaned up\n");
}