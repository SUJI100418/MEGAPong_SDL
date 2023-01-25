#pragma once
#include "SDL.h"

struct Vector2
{
	float x;
	float y;
};


class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	bool mIsRunning;

	Uint32 mTicksCount;

	// Paddle
	Vector2 mPaddlePos;
	int mPaddleDir;

	// Ball
	Vector2 mBallPos;
	Vector2 mBallVel;
};