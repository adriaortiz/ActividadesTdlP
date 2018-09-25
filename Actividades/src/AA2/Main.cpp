
#include "../../dep/inc/SDL_image.h"


SDL_Renderer *renderer = nullptr;		//Treballa com a buffer
SDL_Window *window = nullptr;
//------------INIT-------------
if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw “No es pot inicialitzar SDL”;	//Si no s’ ha pogut executar correctament, imprimeix aixo



//-----------WINDOW-------
m_window = SDL_CreateWindow(“My first SDL project”, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//Nom, on la volem i l’ estat de la finestra
if (m_window == nullptr) throw “No es pot inicialitzar SDL_Window”;	//Si no es pot crear, que s’ execute un throw 
//--------RENDERER-----------
m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//SDL ens permet tenir diverses finestres, pero cada finestra te de tenir un sol renderer, per tant, m_renderer es responsable de m_window
if (m_renderer == nullptr) throw “No es pot inicialitzar SDL_Renderer”;


int main() {
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer,"../../res/img/Gamer.jpg")};
	if (bgTexture == nullptr) throw “No s’ ha pogut crear la textura bgTexture”;
	SDL_Rect bgRect{ 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//Posicio i dimensions 
	//*********************GAMELOOP************************//
	SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
	SDL_RenderPresent(renderer);				//Pintar la textura 
	//Aquí faltaría un SDL_RenderClean(renderer);
	//*********************************************//
	SDL_DestroyTexture(bgTexture);
}
