// SpaceInvaders_Tobias_Mars_Eksamen.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "TobysSpaceInvaderGame.h"

int main(int argc, char * argv[])
{
	// Sjekker om SDL er riktig innstalert...
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL var ikke riktig installert. SDL Error: " << SDL_GetError() << endl;
	}
	else
	{
		cout << "SDL var riktig installert!" << endl;
	}

	TobysSpaceInvaderGame game;
	
	return 0;
}

