
#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
{

}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// 윈도우 생성 : 종이
	mWindow = SDL_CreateWindow(
		"MEGA Pong",
		100,
		100,
		1024,
		768,
		0
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window : %s", SDL_GetError());
		return false;
	}

	// 렌더러 생성 : 붓, 연필
	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer : %s", SDL_GetError());
		return false;
	}

	// 위치 설정
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;

	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;

	return true;
}


void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}


	// 키보드 esc
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}


void Game::UpdateGame()
{

}


void Game::GenerateOutput()
{
	// 후면 버퍼 색상 설정
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);

	// 1. 후면 버퍼를 단색으로 지움
	SDL_RenderClear(mRenderer);

	// 2. 전체 게임 장면 그림

	// 2-1. 벽 그리기
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255); // 붓 색깔 교체
	SDL_Rect wall{ 
		0,         // 왼쪽 상단 x
		0,         // 왼쪽 상단 y
		1024,      // 너비
		thickness  // 높이
	};

	SDL_RenderFillRect(mRenderer, &wall);  // 상단벽

	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);  // 하단벽

	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;

	SDL_RenderFillRect(mRenderer, &wall); // 오른쪽 벽

	// 2-2. 패들 그리기
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // 붓 색깔 교체

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	// 2-3. 공 그리기
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	// 3. 전면 버퍼 <-> 후면 버퍼 swap 교체
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	// 붓 치우고 -> 종이 치우고 -> 종료
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}