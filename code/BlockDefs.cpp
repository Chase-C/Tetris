#include "BlockDefs.h"
#include "CBlock.h"

// This file contains all of the block definitions. If you want to change one or add your own, do it here.
// Don't forget to update BlockDefs.h if you add a new block type.

// returns a 2D, 4x4 array
char** CreateArray()
{
	char** newArray = new char*[4];
	for(int i = 0; i < 4; i++) {
		newArray[i] = new char[4];
		for(int j = 0; j < 4; j++)
			newArray[i][j] = 0;
	}

	return newArray;
}

CBlock* IBlock()
{
	CBlock* temp = new CBlock();

	char** def1 = CreateArray();
	char** def2 = CreateArray();

	for(int i = 0; i < 4; i++) {	// def1 =  {{0, 1, 0, 0},		def2 = {{0, 0, 0, 0},
		def1[i][1] = 1;				//			{0, 1, 0, 0},				{0, 0, 0, 0},
		def2[2][i] = 1;				//			{0, 1, 0, 0},				{1, 1, 1, 1},
	}								//			{0, 1, 0, 0}}				{0, 0, 0, 0}}

	temp->Define(def1);
	temp->DefineRotation(def2);

	return temp;
}

CBlock* OBlock() // the letter O, not 0
{
	CBlock* temp = new CBlock();
	char** def1 = CreateArray();

	def1[0][0] = 1;				// def1 =  {{1, 1, 0, 0},
	def1[1][0] = 1;				//			{1, 1, 0, 0},
	def1[0][1] = 1;				//			{0, 0, 0, 0},
	def1[1][1] = 1;				//			{0, 0, 0, 0}}

	temp->Define(def1);

	return temp;
}

CBlock* TBlock()
{
	CBlock* temp = new CBlock;
	char** def1 = CreateArray();
	char** def2 = CreateArray();
	char** def3 = CreateArray();
	char** def4 = CreateArray();

	for(int i = 0; i < 3; i++) {
		def1[1][i] = 1;				//	def1 = {{0, 1, 0, 0},		def2 = {{0, 1, 0, 0},
		def2[i][1] = 1;				//			{1, 1, 1, 0},				{0, 1, 1, 0},
		def3[1][i] = 1;				//			{0, 0, 0, 0},				{0, 1, 0, 0},
		def4[i][1] = 1;				//			{0, 0, 0, 0}}				{0, 0, 0, 0}}
	}

	def1[0][1] = 1;					//	def3 = {{0, 0, 0, 0},		def4 = {{0, 1, 0, 0},
	def2[1][2] = 1;					//			{1, 1, 1, 0},				{1, 1, 0, 0},
	def3[2][1] = 1;					//			{0, 1, 0, 0},				{0, 1, 0, 0},
	def4[1][0] = 1;					//			{0, 0, 0, 0}}				{0, 0, 0, 0}}

	temp->Define(def1);
	temp->DefineRotation(def2, def3, def4);

	return temp;
}

CBlock* LBlock()
{
	CBlock* temp = new CBlock;
	char** def1 = CreateArray();
	char** def2 = CreateArray();
	char** def3 = CreateArray();
	char** def4 = CreateArray();

	for(int i = 0; i < 3; i++) {
		def1[i][0] = 1;				//	def1 = {{1, 0, 0, 0},		def2 = {{1, 1, 1, 0},
		def2[0][i] = 1;				//			{1, 0, 0, 0},				{1, 0, 0, 0},
		def3[i][1] = 1;				//			{1, 1, 0, 0},				{0, 0, 0, 0},
		def4[1][i] = 1;				//			{0, 0, 0, 0}}				{0, 0, 0, 0}}
	}

	def1[2][1] = 1;					//	def3 = {{1, 1, 0, 0},		def4 = {{0, 0, 1, 0},
	def2[1][0] = 1;					//			{0, 1, 0, 0},				{1, 1, 1, 0},
	def3[0][0] = 1;					//			{0, 1, 0, 0},				{0, 0, 0, 0},
	def4[0][2] = 1;					//			{0, 0, 0, 0}}				{0, 0, 0, 0}}

	temp->Define(def1);
	temp->DefineRotation(def2, def3, def4);

	return temp;
}

CBlock* JBlock()
{
	CBlock* temp = new CBlock;
	char** def1 = CreateArray();
	char** def2 = CreateArray();
	char** def3 = CreateArray();
	char** def4 = CreateArray();

	for(int i = 0; i < 3; i++) {
		def1[i][1] = 1;				//	def1 = {{0, 1, 0, 0},		def2 = {{1, 0, 0, 0},
		def2[1][i] = 1;				//			{0, 1, 0, 0},				{1, 1, 1, 0},
		def3[i][0] = 1;				//			{1, 1, 0, 0},				{0, 0, 0, 0},
		def4[0][i] = 1;				//			{0, 0, 0, 0}}				{0, 0, 0, 0}}
	}

	def1[2][0] = 1;					//	def1 = {{0, 1, 0, 0},		def2 = {{1, 0, 0, 0},
	def2[0][0] = 1;					//			{0, 1, 0, 0},				{1, 1, 1, 0},
	def3[0][1] = 1;					//			{1, 1, 0, 0},				{0, 0, 0, 0},
	def4[1][2] = 1;					//			{0, 0, 0, 0}}				{0, 0, 0, 0}}

	temp->Define(def1);
	temp->DefineRotation(def2, def3, def4);

	return temp;
}

CBlock* SBlock()
{
	CBlock* temp = new CBlock;
	char** def1 = CreateArray();
	char** def2 = CreateArray();

	for(int i = 0; i < 2; i++) {	//	def1 = {{0, 1, 1, 0},		def2 = {{1, 0, 0, 0},
		def1[1][i] = 1;				//			{1, 1, 0, 0},				{1, 1, 0, 0},
		def1[0][i+1] = 1;			//			{0, 0, 0, 0},				{0, 1, 0, 0},
									//			[0, 0, 0, 0}				{0, 0, 0, 0}}
		def2[i][0] = 1;
		def2[i+1][1] = 1;
	}

	temp->Define(def1);
	temp->DefineRotation(def2);

	return temp;
}

CBlock* ZBlock()
{
	CBlock* temp = new CBlock;
	char** def1 = CreateArray();
	char** def2 = CreateArray();

	for(int i = 0; i < 2; i++) {
		def1[0][i] = 1;				//	def1 = {{1, 1, 0, 0},		def2 = {{0, 1, 0, 0},
		def1[1][i+1] = 1;			//			{0, 1, 1, 0},				{1, 1, 0, 0},
									//			{0, 0, 0, 0},				{1, 0, 0, 0},
		def2[i][1] = 1;				//			[0, 0, 0, 0}				{0, 0, 0, 0}}
		def2[i+1][0] = 1;
	}

	temp->Define(def1);
	temp->DefineRotation(def2);

	return temp;
}