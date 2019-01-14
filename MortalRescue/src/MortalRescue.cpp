// MortalRescue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "game.h"

Game* game = 0;

int main(int argc, char *args[])

{
    std::cout << "Mortal Rescue Begins\n"; 

	game = new Game();
	game->init();

	float fps = 0.;
	//float milisecsPerUpdate = 16;
	
	while (game->running()) {

		game->clock.tick();

		game->handleEvents();
		game->update();
		game->render();

		game->clock.tick();

		game->clock.current_frame_cnt++;
		game->clock.calcFps();

		/*
		std::cout << "Clock Duration was " << game->clock.time_diff.count() << " ns\n";
		std::cout << "FPS time accum is " << game->clock.fps_time_accum.count() << " ns\n";
		std::cout << "FPS count      is " << game->clock.current_frame_cnt << " \n";
		std::cout << "FPS is " << game->clock.fps << "\n";
		*/
		//std::cout << "FPS is " << game->clock.fps << "\n";
		
	}
	
	game->clean();

	return 0;

}


