#ifndef GRAPHICS_H
#define GRAPHICS_H

/* Graphics class
 * Holds all information d ealing with graphics for the game
 */

#include <string>
#include <map>

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();
	/* SDL_Surface* loadImage
	 * Loads an image into the _spriteSheets map if it doesn't already exist.
	 * As a result, each image will only be loaded once
	 * returns the image from the map regardless of whether or not it was just loaded
	 */
	SDL_Surface* loadImage(const std::string &filePath);
	/*
	 * draws a texture to a certain part of the screen
	 */
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

	/*
	 * void flip
	 * renders everything to the screen
	 */
	void flip();

	/*
	 * void clear
	 * clears the screen
	 */
	void clear();

	/*
	 * SDL_Renderer* getRenderer
	 * Returns the renderer so we can reuse it.
	 */
	SDL_Renderer* getRenderer() const;

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;

	std::map<std::string, SDL_Surface*> _spriteSheets;
};
#endif
