#ifndef CBLOCK_H
#define CBLOCK_H

class CBlock
{
public:
	bool	canRotate,
			canMoveLeft,
			canMoveRight,
			canMoveDown;

	void Init();

	void Define(char** d);
	void DefineRotation(char** d);
	void DefineRotation(char** d1, char** d2, char** d3);

	void SetColor(int c);

	void Cleanup();

	void Rotate();
	void MoveLeft();
	void MoveRight();
	void MoveDown();

	int GetX();
	int GetY();

	double GetTimeAlive();

	char** GetDef();
	char** GetRotDef();

	CBlock() { }

private:
	int x, y;
	double time;

	char*** def;

	int rotation;
	int numRotations;
};

#endif