#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
using std::cout;
using std::endl;
using std::string;
#include <SDL.h>
#include <SDL_image.h>

class GameObject 
{
public:
	GameObject() { solid = 1; }
	SDL_Rect getDest() const { return dest; }
	SDL_Rect getSource() const { return src; }
	void setDest(int x, int y, int w, int h);
	void setSource(int x, int y, int w, int h);
	void setImage(string filename, SDL_Renderer* ren);
	SDL_Texture* getTex() const { return tex; }
	void setSolid(bool s) { solid = s; }
	bool getSolid()const { return solid; }
	SDL_Texture* tex;
	SDL_Rect dest;
	int tempDestX;
	int tempDestY;
private:
	SDL_Rect src;
	bool solid;
};

#endif