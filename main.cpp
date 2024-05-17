#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "logic.h"
#include "def.h"
#include "graphics.h"

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

int main(int argc, char* args[]) {
         Graphics graphics;
    graphics.init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 0);
    Mix_Music *gMusic = graphics.loadMusic("nhacnen.mp3");
    graphics.play(gMusic);
    SDL_Texture* background = graphics.loadTextures("background9.jpg");
    graphics.prepareScene(background);
    graphics.presentScene();
	SDL_Delay(1000);

  bool click=false;
    int x,y;
    SDL_Event event;
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&x, &y);
                if(x>395&&x<564&&y>441&&y<493)
     {SDL_DestroyTexture(background);
    background = NULL;
    graphics.quit();
		SDL_Window* window = SDL_CreateWindow("Checkers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, boardSize, boardSize, 0);
			SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
				SDL_RendererInfo rendererInfo;
				SDL_GetRendererInfo(renderer, &rendererInfo);
				std::cout << "Renderer = " << rendererInfo.name << std::endl;
				Game game(window, renderer, boardSize);
				SDL_DestroyRenderer(renderer);
			    SDL_DestroyWindow(window);
		}
		else if (x>384&x<570&y>321&y<381)
        {
      SDL_Texture* spongeBob = graphics.loadTextures("lc3.png" );
    graphics.renderTexture(spongeBob, 75, 75);
    graphics.presentScene();
	SDL_Delay(1000);

        }}}

		}

 if (gMusic != nullptr) Mix_FreeMusic( gMusic );
 graphics.quit();
	return 0;
}
