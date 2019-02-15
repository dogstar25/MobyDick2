// MortalRescue.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "game.h"
#include <math.h>

Game* game = 0;
using namespace std;

int main(int argc, char *args[])

{
    cout << "Mortal Rescue Begins\n"; 

	game = new Game();
	game->init();

	float fps = 0.;

	//TIMESTEP TEST
	
	std::chrono::duration<double> milisecsPerUpdate = std::chrono::milliseconds(16);
	steady_clock::time_point previous_time = steady_clock::now();
	std::chrono::duration<double> lag = std::chrono::milliseconds(0);
	std::chrono::duration<double> time_diff = std::chrono::milliseconds(0);
	
	/*
	Main Game loop
	*/
	while (game->running()) {

		game->clock.tick();

		//Calculate how many steps to update physics and objects
		steady_clock::time_point current_time = steady_clock::now();
		std::chrono::duration<double> time_diff = current_time - previous_time;
		previous_time = current_time;
		int stepCount = floor(milisecsPerUpdate / time_diff) + 1;
		stepCount = min(stepCount, 5);
		//cout << "stepCount is " << stepCount << "\n";

		//handle input
		game->handleEvents();

		//temp
		//stepCount = 1;

		//Handle updating objects positions and physics
		for (int step = 0; step < stepCount; step++)
		{
			game->update();
		}

		//render everything
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
		std::cout << "FPS is " << game->clock.fps << "\n";
		
	}
	
	game->clean();

	return 0;

}


