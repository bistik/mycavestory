#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <map>
#include <string>
#include <vector>
#include "globals.h"
#include "sprite.h"

class Graphics;

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
			float posX, float posY, float timeToUpdate);
	/*
	 * plays animation if its not already playing
	 */
	void playAnimation(std::string animation, bool once = false);

	/*
	 * update the animated sprite with a timer
	 */
	void update(int elapsedTime);

	/*
	 * draws the sprite to the screen
	 */
	void draw(Graphics &graphics, int x, int y);

protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	/*
	 * add an animation to the map of animations for the sprite
	 */
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	/*
	 * resets all animations
	 */
	void resetAnimations();

	/*
	 * stops the current animation
	 */
	void stopAnimation();

	/*
	 * changes the visibility of the animated sprite
	 */
	void setVisible(bool visible);

	/*
	 * happens when an animation ends
	 */
	virtual void animationDone(std::string currentAnimation) = 0;

	/*
	 * setup all animations for a sprite
	 */
	virtual void setupAnimations() = 0;
private:
	std::map<std::string, std::vector<SDL_Rect> > _animations;
	std::map<std::string, Vector2> _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visible;
};
#endif
