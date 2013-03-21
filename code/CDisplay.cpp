#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>

#include "CBlock.h"
#include "BlockDefs.h"
#include "CDisplay.h"

#define GRIDCOLOR	al_map_rgb(180, 180, 180)
#define COLOR1		al_map_rgba(0, 0, 0, 205)
#define COLOR2		al_map_rgba(0, 0, 0, 180)
#define COLOR3		al_map_rgba(0, 0, 0, 160)
#define COLOR4		al_map_rgba(0, 0, 0, 130)
#define COLOR5		al_map_rgba(0, 0, 0, 90)

void DrawBlock(int color, int bx, int by);

void CDisplay::Init()
{
	font = al_load_font("RevoPop.ttf",14,0);
	if(!font) {
		font = al_load_font("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\RevoPop.ttf",14,0);

		if(!font)
			fprintf(stderr, "Could Not load font");
	}

	BlockOrder = new CBlock*[7];
	blockIndex = 0;

	level = 1;
	score = 0;
	lines = 0;

	// set random seed
	srand(time(NULL));

	ReadFile();

	GenerateNewBlockOrder();
	nextBlock = BlockOrder[0];
}

void CDisplay::Cleanup()
{
	for(int i = 0; i < 7; i++) {
		if(BlockOrder[i])
			delete BlockOrder[i];
	}
	delete[] BlockOrder;

	for(int i = 0; i < numHighScores; i++)
		delete highScores[i];
	delete[] highScores;

	printf("Display cleaned up\n");
}

CBlock* CDisplay::NextBlock()
{
	CBlock* tempBlock = nextBlock;
	BlockOrder[blockIndex] = NULL;
	blockIndex++;

	if(blockIndex >= 7) {
		GenerateNewBlockOrder();
		blockIndex = 0;
	}

	nextBlock = BlockOrder[blockIndex];
	return tempBlock;
}

void CDisplay::GenerateNewBlockOrder()
{
	bool placed[7];
	for(int i = 0; i < 7; i++)
		placed[i] = false;

	for(int i = 0; i < 7; i++) {
		int r;
		do {
			r = rand() % 7;
		} while(placed[r]);

		placed[r] = true;

		switch(r) {
		case 0:
			BlockOrder[i] = IBlock();
			break;

		case 1:
			BlockOrder[i] = OBlock();
			break;

		case 2:
			BlockOrder[i] = TBlock();
			break;

		case 3:
			BlockOrder[i] = LBlock();
			break;

		case 4:
			BlockOrder[i] = JBlock();
			break;

		case 5:
			BlockOrder[i] = SBlock();
			break;

		case 6:
			BlockOrder[i] = ZBlock();
			break;
		}

		BlockOrder[i]->Init();
		BlockOrder[i]->SetColor((rand() % 5) + 1);
	}
}

void CDisplay::AddToScore(int amount)
{
	score += amount;
}

void CDisplay::AddToLines(int amount)
{
	lines += amount;
}

void CDisplay::IncreaseLevel()
{
	level++;
}

long CDisplay::GetScore()
{
	return score;
}

char* CDisplay::GetScoreString()
{
	return ltoa(score, new char[32], 10);
}

void CDisplay::UpdateHighScores()
{
	for(int i = 0; i < numHighScores; i++) {
		long tempScore = atol(highScores[i]);
		
		if(score > tempScore) {
			for(int j = numHighScores - 1; j > i; j--)
				highScores[j] = highScores[j - 1];

			highScores[i] = ltoa(score, new char[16], 10);
			break;
		}
	}

	ALLEGRO_FILE* file;
	file = al_fopen("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\scores.scr", "w+b");
	if(!file) {
		printf("File scores.scr could not be opened\n");
		return;
	}

	for(int i = 0; i < numHighScores; i++) {
		long tempScore = atol(highScores[i]);
		al_fwrite(file, &tempScore, 8);
	}

	al_fclose(file);

	printf("File written to");
}

void CDisplay::ReadFile()
{
	ALLEGRO_FILE* file;
	file = al_fopen("C:\\Users\\Chase\\Documents\\Visual Studio 2010\\Projects\\Tetris\\scores.scr", "rb");
	if(!file)
		printf("File scores.scr could not be opened\n");

	numHighScores = al_fsize(file) / 8;
	highScores = new char*[numHighScores];

	for(int i = 0; i < numHighScores; i++) {
		long* temp = new long;
		al_fread(file, temp, 8);
		highScores[i] = ltoa(*temp, new char[16], 10);
	}

	al_fclose(file);

	printf("File read from, scores: %d\n",numHighScores);
}

void CDisplay::Draw()
{
	// convert the variables into strings to be drawn to the screen
	char* tempScore = ltoa(score, new char[sizeof(long) * 8 + 1], 10);

	char* tempLines = itoa(lines, new char[sizeof(int) * 8 + 1], 10);
	char* tempLevel = itoa(level, new char[sizeof(int) * 8 + 1], 10);
	
	al_draw_text(font, al_map_rgb(32,32,32), 78, 206, ALLEGRO_ALIGN_LEFT, tempScore);
	al_draw_text(font, al_map_rgb(32,32,32), 78, 224, ALLEGRO_ALIGN_LEFT, tempLines);
	al_draw_text(font, al_map_rgb(32,32,32), 78, 262, ALLEGRO_ALIGN_LEFT, tempLevel);

	// draw the next block
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(nextBlock->GetDef()[i][j] != 0) {
				DrawBlock(nextBlock->GetDef()[i][j], 
						((j + nextBlock->GetX()) * 32) + 30, 
						((i + nextBlock->GetY()) * 32) + 30);
			}
		}
	}

	for(int i = 0; i < numHighScores; i++) {
		al_draw_text(font, al_map_rgb(0, 0, 0), 26, 360 + (24 * i), 0, itoa(i + 1, new char[2], 10));
		al_draw_text(font, al_map_rgb(0, 0, 0), 44, 360 + (24 * i), 0, highScores[i]);
	}
}

void DrawBlock(int color, int bx, int by)
{
	switch(color) {
	case 1:
		al_draw_filled_rectangle(bx, by, bx + 32, by + 32, COLOR1);
		break;

	case 2:
		al_draw_filled_rectangle(bx, by, bx + 32, by + 32, COLOR2);
		break;

	case 3:
		al_draw_filled_rectangle(bx, by, bx + 32, by + 32, COLOR3);
		break;

	case 4:
		al_draw_filled_rectangle(bx, by, bx + 32, by + 32, COLOR4);
		break;

	case 5:
		al_draw_filled_rectangle(bx, by, bx + 32, by + 32, COLOR5);
		break;
	}
}