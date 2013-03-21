#ifndef CGRID_H
#define CGRID_H

#include <allegro5\allegro.h>

class CBlock;
class CDisplay;

class CGrid
{
public:
	bool GameOver();
	bool NeedBlock();

	void Init(int X, int Y, int nCols, int w, int h);
	void Cleanup();

	void AddBlock(CBlock* block);

	void RotateBlock();
	void MoveBlockLeft();
	void MoveBlockRight();
	void DropBlock();

	void AccelerateBlock();
	void DecelerateBlock();

	void Update(CDisplay* display);
	void Draw();

	CGrid() { }

private:
	bool gameOver;

	int x, y;
	int numRows, numCols;
	double blockLength;

	ALLEGRO_BITMAP* c_grid_container;
	char** c_grid;

	CBlock* activeBlock;
	bool needBlock;

	int level;
	double fallDelay;
	double time;

	void UpdateActiveBlock();
	void CopyBlockToGrid();

	void RemoveBlock(CDisplay* display);

	void CheckLines(CDisplay* display);
	void RemoveLine(int line);

	void IncreaseLevel(CDisplay* display);

	void DrawBlock(int color, int bx, int by);
};

#endif