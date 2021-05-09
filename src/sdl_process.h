#ifndef MY_SDL_PROCESS_H
#define MY_SDL_PROCESS_H

#include <SDL.h>
#include <iostream>

using namespace std;

struct MainWindow{
    struct Config{
        int width = 800, height = 600;
        float fps = 60.0f;
    } config;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Uint32 tick = 0;
	bool need_end = false;
};

bool init(MainWindow& main_window_){
    auto& window = main_window_.window;
    auto& renderer = main_window_.renderer;
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	} 

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED
        , SDL_WINDOWPOS_UNDEFINED, main_window_.config.width
        , main_window_.config.height, SDL_WINDOW_SHOWN );
	if ( !window ) {
		cout << "Error creating window: " << SDL_GetError()  << endl;
		system("pause");
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
	return true;
}

void kill(MainWindow& main_window_){
	SDL_DestroyRenderer( main_window_.renderer );
	SDL_DestroyWindow( main_window_.window );
	SDL_Quit();
}

bool pre_loop(MainWindow& main_window_){
	SDL_SetRenderDrawColor( main_window_.renderer, 255, 255, 255, 255 );
	SDL_RenderClear( main_window_.renderer );
    main_window_.tick = SDL_GetTicks();
}

bool pro_loop(MainWindow& main_window_){
	SDL_RenderPresent( main_window_.renderer );
    auto now_tick = SDL_GetTicks();
    if (now_tick - main_window_.tick < 1000.0f / main_window_.config.fps)
        SDL_Delay(1000 / main_window_.config.fps - now_tick + main_window_.tick);
}

#endif // ! MY_SDL_PROCESS_H