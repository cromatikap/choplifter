#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "initialisation.h"
#include <SDL/SDL_getenv.h>
#include <fmodex/fmod.h>
#include "time.h"


int main(int argc, char *argv[])
{
    freopen("CON","w", stderr);
    freopen("CON","w", stdout);
	menu();
	return 0;
}
