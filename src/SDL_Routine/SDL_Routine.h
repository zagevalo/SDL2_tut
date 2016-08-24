#pragma once

class SDL_Routine {
	public:
		SDL_Routine();
		int InitSDL();
		int CleanupSDL();

		SDL_Renderer *Render;
		SDL_Window *MainWind;

	private:

};
