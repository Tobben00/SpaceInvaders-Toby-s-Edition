#include "stdafx.h"
#include "TobysSpaceInvaderGame.h"
#include "GameObject.h"
#include "Audio.h"
#include <string>

TobysSpaceInvaderGame::TobysSpaceInvaderGame()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(840, 480, 0, &win, &ren);
	SDL_SetWindowTitle(win, "Tobys Space Invader v1.0");
	TTF_Init();
	score = 0;
	GameOver = false;
	playerHasUsedGun = false;
	shipMoveRight = true;

	soundEffect.load("@..//..//assets//audio//shipgun.wav");
	blastEffect.load("@..//..//assets//audio//blast.wav");
	
	font = TTF_OpenFont("@..//..//assets//fonts//pdark.ttf", 24);

	// Player Settings
	Player.tempDestX = 400;
	Player.setDest(Player.tempDestX, 410, 55, 40);
	Player.setSource(0, 0, 59, 42); // 59 42
	Player.setImage("@..//..//assets//bmp//playerShip.bmp", ren);

	// Projectile / Lazer settings
	Projectile.tempDestY = 420;
	Projectile.setDest(Player.tempDestX, 420, 20, 25);
	Projectile.setSource(0, 0, 59, 42);
	Projectile.setImage("@..//..//assets//bmp//lazer.bmp", ren);

	// Enemy1 Settings
	Enemy1.tempDestX = 30;
	Enemy1.tempDestY = 75;
	Enemy1.setDest(Enemy1.tempDestX, 75, 59, 42);
	Enemy1.setSource(0, 0, 59, 42);
	Enemy1.setImage("@..//..//assets//bmp//enemyShipSmall.bmp", ren);

	loop();
}

TobysSpaceInvaderGame::~TobysSpaceInvaderGame()
{
	TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void TobysSpaceInvaderGame::loop() 
{
	soundEffect.load("@..//..//assets//audio//shipgun.wav");
	while(!GameOver)
	{
		lastFrame = SDL_GetTicks();
		static int lastTime;

		// Får ut tiden i sekkunder = 1000 millisekkunder 
		if(lastFrame >= (lastTime + 1000))
		{
			lastTime = lastFrame;
			frameCount = 0;
		}

		render();
		input();
		enemyMoveToWinRight();
		enemyMoveToWinLeft();
		update();
	}
}

void TobysSpaceInvaderGame::render() 
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_Rect rect;
	rect.x = rect.y = 0;
	rect.w = 840;
	rect.h = 480;
	SDL_RenderFillRect(ren, &rect);

	draw(Enemy1);

	draw(Projectile);
	draw(Player);
	
	std::string s = std::to_string(score);
	char const *pchar = s.c_str();
	
	drawTTF("Score", 5, 5, 0, 255, 0);
	drawTTF(pchar, 100, 5, 0, 255, 0);

	drawTTF("LEVEL", 140, 5, 0, 255, 0);
	drawTTF("1", 235, 5, 0, 255, 0);

	frameCount++;
	timerFps = SDL_GetTicks() - lastFrame;
	if (timerFps < (1000 / 60))
	{
		SDL_Delay(1000 / 60 - timerFps);
	}

	SDL_RenderPresent(ren);
}

void TobysSpaceInvaderGame::input() 
{
	// Nå kan vi sjekke etter tastetrykk :D
	SDL_PumpEvents();
	const Uint8* keys = SDL_GetKeyboardState(&numKeys);
	
	SDL_Event e;

	SDL_PollEvent(&e);
	
	if(e.type == SDL_KEYUP) 
	{
		int tempTime = lastFrame;
		if (e.key.keysym.sym == SDLK_SPACE && playerHasUsedGun == false) 
		{
			soundEffect.play();
			cout << "PANG!!!" << endl; // Avfyr kannonene!!! PANG!!
			Projectile.tempDestX = Player.tempDestX + 17;
			playerHasUsedGun = true;
		} 
	}
	
	if (SDL_HasEvent(SDL_QUIT))
	{
		GameOver = true; // Avslutt
	}

	if (keys[SDL_SCANCODE_RIGHT] != 0)
	{
		if (Player.tempDestX >= 783)
			Player.tempDestX = 782;

		Player.setDest(Player.tempDestX++, 410, 55, 40);
		cout << "Går til høyere: " << Player.tempDestX << endl; // Bevege seg til høyere
	}

	if (keys[SDL_SCANCODE_LEFT] != 0)
	{
		if (Player.tempDestX <= 0)
			Player.tempDestX = 1;

		Player.setDest(Player.tempDestX--, 410, 55, 40);
		cout << "Går til venstere: "<< Player.tempDestX << endl; // Bevege seg til venstere
	}
}

void TobysSpaceInvaderGame::update() 
{
	if (checkCollison(Enemy1)) 
	{
		blastEffect.play();
		SDL_DestroyTexture(Projectile.tex);
		SDL_DestroyTexture(Enemy1.tex);

		Projectile.tempDestY = 390;
		Projectile.setDest(Player.tempDestX, 420, 20, 25);
		Projectile.setSource(0, 0, 59, 42);
		Projectile.setImage("@..//..//assets//bmp//lazer.bmp", ren);
		
		playerHasUsedGun = false;
		score++; // Fikk vi poeng på scoret :D
	}

	if (!playerHasUsedGun)
	{
		Projectile.setDest(Player.tempDestX, 420, 20, 25);
	}

	if (playerHasUsedGun)
	{ 
		--Projectile.tempDestY;
		Projectile.setDest(Projectile.tempDestX, Projectile.tempDestY, 20, 25);
	}

	if(Projectile.tempDestY <= -20) 
	{
		playerHasUsedGun = false;
		
		SDL_DestroyTexture(Projectile.tex);

		Projectile.tempDestY = 390;
		Projectile.setDest(Player.tempDestX, 420, 20, 25);
		Projectile.setSource(0, 0, 59, 42);
		Projectile.setImage("@..//..//assets//bmp//lazer.bmp", ren);
	}
}

void TobysSpaceInvaderGame::draw(GameObject go) 
{
	SDL_Rect dest = go.getDest();
	SDL_Rect src = go.getSource();
	SDL_RenderCopyEx(ren, go.getTex(), &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

void TobysSpaceInvaderGame::drawTTF(const char* msg, int x, int y, int r, int g, int b) 
{
	SDL_Surface* surf;
	SDL_Texture* tex;
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = 255;
	SDL_Rect rect;
	surf = TTF_RenderText_Solid(font, msg, color);
	tex = SDL_CreateTextureFromSurface(ren, surf);
	rect.x = x;
	rect.y = y;
	rect.w = surf->w;
	rect.h = surf->h;
	SDL_FreeSurface(surf);
	SDL_RenderCopy(ren, tex, NULL, &rect);
	SDL_DestroyTexture(tex);
}

bool TobysSpaceInvaderGame::checkCollison(GameObject enemy)
{
	if(Projectile.dest.x + Projectile.dest.w < enemy.dest.x || Projectile.dest.x > enemy.dest.x + enemy.dest.w ||
		Projectile.dest.y + Projectile.dest.h < enemy.dest.y || Projectile.dest.y > enemy.dest.y + enemy.dest.h) 
	{
		// Dette skjer når ingenting er truffet
		return false;
	}
	else 
	{
		cout << "TRUFFET SKIP!!!" << endl;
		return true;
	}
}

void TobysSpaceInvaderGame::enemyMoveToWinRight()
{ 
	if (Enemy1.tempDestX <= 780 && shipMoveRight == true) { Enemy1.setDest(Enemy1.tempDestX++, Enemy1.tempDestY, 55, 40); }
	
	if (Enemy1.tempDestX == 780) {
		shipMoveRight = false; 
	}
}  

void TobysSpaceInvaderGame::enemyMoveToWinLeft()
{
	if (shipMoveRight == false) { Enemy1.setDest(Enemy1.tempDestX--, Enemy1.tempDestY, 55, 40); }
	
	if (Enemy1.tempDestX == 10)
	shipMoveRight = true;
}