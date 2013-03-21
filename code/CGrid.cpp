#include <stdio.h>
#include <cmath>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#include "CGrid.h"
#include "CBlock.h"
#include "CDisplay.h"
#include "BlockDefs.h"

#define GRIDCOLOR	al_map_rgb(180, 180, 180)
#define COLOR1		al_map_rgba(0, 0, 0, 205)
#define COLOR2		al_map_rgba(0, 0, 0, 180)
#define COLOR3		al_map_rgba(0, 0, 0, 160)
#define COLOR4		al_map_rgba(0, 0, 0, 130)
#define COLOR5		al_map_rgba(0, 0, 0, 90)

void CGrid::Init(int X, int Y, int nCols, int w, int h)
{
	gameOver = false;

	x = X;
	y = Y;

	numCols = nCols;
	numRows = (h / w) * numCols;

	blockLength = w / numCols;

	c_grid_container = al_create_bitmap(w, h);

	ALLEGRO_BITMAP* temp = al_get_target_bitmap();

	al_set_target_bitmap(c_grid_container);
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));

	// draw grid lines to the bitmap
	for(int i = 1; i < numCols; i++) {
		int dx = i*blockLength;
		al_draw_line(dx, 0, dx, h, GRIDCOLOR, 1);
	}

	for(int i = 1; i < numRows; i++) {
		int dy = i*blockLength;
		al_draw_line(0, dy, w - 1, dy, GRIDCOLOR, 1);
	}

	al_set_target_bitmap(temp);

	// create and initialize the grid
	c_grid = new char*[numRows];			// \ 
	for(int i = 0; i < numRows; i++) {		// - Create
		c_grid[i] = new char[numCols];		// /
		for(int j = 0; j < numCols; j++)	// \ 
			c_grid[i][j] = 0;				// - Initialize (set all char's = to 0)
	}										// /

	activeBlock = NULL;
	needBlock = true;

	level = 1;

	fallDelay = 0.6;
	time = al_get_time();

	printf("CGrid initialized\n");
}

void CGrid::Cleanup()
{
	// delete the grid
	for(int i = 0; i < numRows; i++)
		delete[] c_grid[i];
	delete[] c_grid;

	activeBlock->Cleanup();
	delete activeBlock;

	al_destroy_bitmap(c_grid_container);

	printf("CGrid cleaned up\n");
}

void CGrid::Update(CDisplay* display)
{
	if(al_get_time() - time >= fallDelay) {
		if(!activeBlock) {
			needBlock = true;
		} else if(activeBlock->canMoveDown) {
			activeBlock->MoveDown();
			UpdateActiveBlock();
		} else {
			CopyBlockToGrid();
			RemoveBlock(display);

			CheckLines(display);

			if(display->GetScore() > 1250 * pow(2, (long double) level - 1))
				IncreaseLevel(display);
		}

		if(needBlock) {
			AddBlock(display->NextBlock());
			needBlock = false;
		}

		time = al_get_time();
	}
}

void CGrid::UpdateActiveBlock()
{
	int tempX = activeBlock->GetX();
	int tempY = activeBlock->GetY();

	activeBlock->canMoveLeft = true;
	activeBlock->canMoveRight = true;
	activeBlock->canMoveDown = true;
	activeBlock->canRotate = true;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(activeBlock->GetDef()[i][j] != 0) {
				// check if the block is able to move to the left
				if(j == 0 || activeBlock->GetDef()[i][j - 1] == 0) {
					if(j + tempX == 0 || c_grid[i + tempY][j + tempX - 1] != 0)
						activeBlock->canMoveLeft = false;
				}

				// check if the block is able to move to the right
				if(j == 3 || activeBlock->GetDef()[i][j + 1] == 0) {
					if(j + tempX == numCols - 1 || c_grid[i + tempY][j + tempX + 1] != 0)
						activeBlock->canMoveRight = false;
				}

				// check if the block is able to move down
				if(i == 3 || activeBlock->GetDef()[i + 1][j] == 0) {
					if(i + tempY == numRows - 1 || c_grid[i + tempY + 1][j + tempX] != 0)
						activeBlock->canMoveDown = false;
				}
			}

			//check if the block is able to rotate
			if(activeBlock->GetRotDef()[i][j] != 0) {
				if((i + tempY >= numRows || (j + tempX >= numCols || j + tempX < 0)) || c_grid[i + tempY][j + tempX] != 0)
					activeBlock->canRotate = false;
			}
		}
	}
}

void CGrid::CheckLines(CDisplay* display)
{
	int numLines = 0;

	for(int i = 0; i < numRows; i++) {
		for(int j = 0; j < numCols; j++) {
			if(c_grid[i][j] == 0)
				break;

			if(j == numCols - 1) {
				RemoveLine(i);
				numLines++;
				display->AddToLines(1);
			}
		}
	}

	for(int i = 0; i < numLines; i++)
		display->AddToScore(500 * (i + 1));
}

void CGrid::RemoveLine(int line)
{
	for(int i = line; i > 0; i--) {
		for(int j = 0; j < numCols; j++) {
			c_grid[i][j] = c_grid[i - 1][j];
		}
	}

	for(int i = 0; i < numCols; i++) {
		c_grid[0][i] = 0;
	}
}

void CGrid::AddBlock(CBlock* block)
{
	if(activeBlock)
		delete activeBlock;

	activeBlock = block;
	activeBlock->Init();

	time = al_get_time();

	UpdateActiveBlock();

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(activeBlock->GetDef()[i][j] != 0) {
				// check if two blocks are overlapping
				if(c_grid[i + activeBlock->GetY()][j + activeBlock->GetX()] != 0)
					gameOver = true;
			}
		}
	}

	needBlock = false;
}

void CGrid::RemoveBlock(CDisplay* display)
{
	double tempTime = 0;

	if(activeBlock) {
		tempTime = activeBlock->GetTimeAlive();

		activeBlock->Cleanup();
		delete activeBlock;
	}

	if(tempTime != 0)
		display->AddToScore((8 * level) + ((int) (10 / (tempTime))));

	activeBlock = NULL;
	needBlock = true;
}

void CGrid::RotateBlock()
{
	if(!activeBlock)
		return;

	if(activeBlock->canRotate) {
		activeBlock->Rotate();
		UpdateActiveBlock();
	}
}

void CGrid::MoveBlockLeft()
{
	if(!activeBlock)
		return;

	if(activeBlock->canMoveLeft) {
		activeBlock->MoveLeft();
		UpdateActiveBlock();
	}
}

void CGrid::MoveBlockRight()
{
	if(!activeBlock)
		return;

	if(activeBlock->canMoveRight) {
		activeBlock->MoveRight();
		UpdateActiveBlock();
	}
}

void CGrid::DropBlock()
{
	if(!activeBlock)
		return;

	while(activeBlock->canMoveDown) {
		activeBlock->MoveDown();
		UpdateActiveBlock();
	}
}

void CGrid::AccelerateBlock()
{
	fallDelay = 0.075;
}

void CGrid::DecelerateBlock()
{
	fallDelay = 0.6 * pow(0.9, level - 1);
}

void CGrid::IncreaseLevel(CDisplay* display)
{
	level++;
	display->IncreaseLevel();
	fallDelay = 0.6 * pow(0.9, level - 1);
}

void CGrid::CopyBlockToGrid()
{
	int bx = activeBlock->GetX();
	int by = activeBlock->GetY();

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(i + by < numRows && (j + bx < numCols && j + bx >= 0))
				c_grid[i +  by][j + bx] += activeBlock->GetDef()[i][j];
		}
	}
}

bool CGrid::GameOver()
{
	return gameOver;
}

bool CGrid::NeedBlock()
{
	return needBlock;
}

void CGrid::Draw()
{
	al_draw_bitmap(c_grid_container, x, y, 0);

	// draw blocks
	for(int i = 0; i < numRows; i++) {
		for(int j = 0; j < numCols; j++) {
			if(c_grid[i][j] != 0)
				DrawBlock(c_grid[i][j], (j * blockLength) + x, (i * blockLength) + y);
		}
	}

	if(activeBlock) {
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				if(activeBlock->GetDef()[i][j] != 0) {
					DrawBlock(activeBlock->GetDef()[i][j], 
							((j + activeBlock->GetX()) * blockLength) + x, 
							((i + activeBlock->GetY()) * blockLength) + y);
				}
			}
		}
	}
}

void CGrid::DrawBlock(int color, int bx, int by)
{
	switch(color) {
	case 1:
		al_draw_filled_rectangle(bx, by, bx + blockLength, by + blockLength, COLOR1);
		break;

	case 2:
		al_draw_filled_rectangle(bx, by, bx + blockLength, by + blockLength, COLOR2);
		break;

	case 3:
		al_draw_filled_rectangle(bx, by, bx + blockLength, by + blockLength, COLOR3);
		break;

	case 4:
		al_draw_filled_rectangle(bx, by, bx + blockLength, by + blockLength, COLOR4);
		break;

	case 5:
		al_draw_filled_rectangle(bx, by, bx + blockLength, by + blockLength, COLOR5);
		break;
	}
}