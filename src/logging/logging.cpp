#include <ostream>
#include <SDL2/SDL.h>

#include "logging.h"

void logSDLError(std::ostream &os, const std::string msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}
