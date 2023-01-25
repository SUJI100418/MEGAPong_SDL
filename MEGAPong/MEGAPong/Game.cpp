
#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mIsRunning(true)
,mTicksCount(0)
,mPaddleDir(0)
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
	
	// ������ ���� : ����
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

	// ������ ���� : ��, ����
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

	// ��ġ ����
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;

	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;

	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;

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


	// Ű���� esc
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Ű���� paddle
	mPaddleDir = 0; // ����
	if (state[SDL_SCANCODE_W])
		mPaddleDir -= 1;
	if (state[SDL_SCANCODE_S])
		mPaddleDir += 1;
}


void Game::UpdateGame()
{
	// ��ǥ �������� 60FPS �̸�, 1�����Ӵ� ��ǥ ��Ÿ �ð��� 16.6ms
	// ����, �������� 15ms ���� ������ �߰��� 1.6ms �� ��ٷ��� �Ѵ�.

	// ������ ������ ���� 16ms �� ����� ������ ���
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));  // �����Ӱ� 16ms �� ������� ����

	// ��Ÿ Ÿ�� ��� : ������  ������ ƽ ���� ���� ������ ƽ���� ��
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	if (deltaTime > 0.05f)
		deltaTime = 0.05f;

	mTicksCount = SDL_GetTicks();  // �ʱ�ȭ ���� ����� �и���

	// ��Ÿ �ð����� ���� ���� ������Ʈ ����

	// 1. �е� �̵�
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
			mPaddlePos.y = paddleH / 2.0f + thickness;

		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
	}

	// 2. �� �̵� -> ?????????
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// �� �浹��?
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.0f) ? diff : -diff;

	if (
		diff <= paddleH / 2.0f &&
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// ���� ��ũ�� ������ ����
	else if (mBallPos.x <= 0.0f)
		mIsRunning = false;

	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
		mBallVel.x *= -1.0f;

	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
		mBallVel.y *= -1;

	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}

}


void Game::GenerateOutput()
{
	// �ĸ� ���� ���� ����
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);

	// 1. �ĸ� ���۸� �ܻ����� ����
	SDL_RenderClear(mRenderer);

	// 2. ��ü ���� ��� �׸�

	// 2-1. �� �׸���
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255); // �� ���� ��ü
	SDL_Rect wall{ 
		0,         // ���� ��� x
		0,         // ���� ��� y
		1024,      // �ʺ�
		thickness  // ����
	};

	SDL_RenderFillRect(mRenderer, &wall);  // ��ܺ�

	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);  // �ϴܺ�

	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;

	SDL_RenderFillRect(mRenderer, &wall); // ������ ��

	// 2-2. �е� �׸���
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); // �� ���� ��ü

	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_RenderFillRect(mRenderer, &paddle);

	// 2-3. �� �׸���
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness/2),
		static_cast<int>(mBallPos.y - thickness/2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball);

	// 3. ���� ���� <-> �ĸ� ���� swap ��ü
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	// �� ġ��� -> ���� ġ��� -> ����
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}