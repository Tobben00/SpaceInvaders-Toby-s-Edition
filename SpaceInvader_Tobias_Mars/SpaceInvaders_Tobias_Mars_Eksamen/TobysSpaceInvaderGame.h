#ifndef TOBYSSPACEINVADERGAME_H
#define TOBYSSPACEINVADERGAME_H

#include <iostream>
using std::cout;
using std::endl;
using std::string;
#include <SDL.h>
#include <SDL_ttf.h>
#include "Audio.h"
#include "GameObject.h"

class TobysSpaceInvaderGame 
{
public:
	TobysSpaceInvaderGame();
	~TobysSpaceInvaderGame();
	void loop();
	void update(); 
	void render(); 
	void input(); 
	void draw(GameObject go);
	void drawTTF(const char* msg, int x, int y, int r, int g, int b);
	bool checkCollison(GameObject enemy);
	void enemyMoveToWinRight();
	void enemyMoveToWinLeft();
	bool playerHasUsedGun;
	bool shipMoveRight;
	int score;
	GameObject Player;
	GameObject Projectile;
	GameObject Enemy1;
private:
	SDL_Renderer * ren;
	SDL_Window * win;
	Audio soundEffect;
	Audio blastEffect;
	TTF_Font *font;
	bool GameOver;
	int numKeys;
	int frameCount;
	int timerFps;
	int lastFrame;
};

#endif // TOBYSSPACEINVADERGAME_H




