#ifndef CDISPLAY_H
#define CDISPLAY_H

#include <allegro5\allegro_font.h>

class CBlock;

class CDisplay
{
public:
	void Init();
	void Cleanup();

	CBlock* NextBlock();

	void AddToScore(int amount);
	void AddToLines(int amount);

	long GetScore();
	char* GetScoreString();

	void IncreaseLevel();

	void UpdateHighScores();

	void Draw();

private:
	ALLEGRO_FONT* font;

	long score;
	int lines;
	int level;

	CBlock* nextBlock;
	CBlock** BlockOrder;
	int blockIndex;

	char** highScores;
	int numHighScores;

	void ReadFile();

	void GenerateNewBlockOrder();
};

#endif